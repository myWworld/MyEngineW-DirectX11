#include "FSMBrainCore.h"

#include "IFSMContext.h"
#include "MEBlackBoard.h"

#include <algorithm>

namespace ME
{
    namespace
    {
        // Update에서 빠져나가는 모든 경로에서
        // mCurrentContext를 nullptr로 복원한다.
        struct ContextResetGuard
        {
            IFSMContext*& contextPointer;

            ~ContextResetGuard()
            {
                contextPointer = nullptr;
            }
        };
    }

    FSMBrainCore::FSMBrainCore()
        : mBlackboard(std::make_unique<BlackBoard>())
    {
    }

    FSMBrainCore::~FSMBrainCore() = default;

    void FSMBrainCore::Update(
        IFSMContext& context) //여기서 받아오는 context는 서버쪽에서 만드는 지역 객체이기에 서버쪽 Update가 끝날씨 소멸하여 여기서 가리키는 것을 댕글링 포인터가 됨 
    {
        if (mActiveState == nullptr &&
            mPendingState == nullptr)
        {
            return;
        }

        // 이 주소는 이 함수 안에서만 보관한다.
        mCurrentContext = &context;

        ContextResetGuard contextGuard
        {
            mCurrentContext
        };

        // Update 밖에서 요청된 상태 전환 적용
        if (mPendingState != nullptr)
        {
            FSMState* pendingState =
                std::exchange(mPendingState,nullptr); //SendFSMEvent같은 요청이 있었을 시 담아두 유효한기간인 update에서만 사용할 수 있도록 

            ApplyStateChange(pendingState,context);
        }

        if (mActiveState == nullptr)
            return;

        // 최초 상태 진입
        if (!mbStarted)
        {
            mStateElapsedTime = 0.0f;

            mActiveState->EnterState(
                this,
                context
            );

            mbStarted = true;
        }

        mStateElapsedTime +=
            (std::max)(
                0.0f,
                context.GetDeltaTime()
                );

        // Task 실행 도중 SendFSMEvent로
        // 상태가 바뀔 수도 있으므로 기존 상태를 저장
        FSMState* stateBeforeTask = mActiveState;

        mActiveState->UpdateTask(
            this,
            context
        );

        if (mActiveState == nullptr)
            return;

        // Task에서 이미 상태가 바뀌었다면
        // 새 상태의 Decision까지 같은 Tick에 검사하지 않는다
        if (mActiveState != stateBeforeTask)
            return;

        mActiveState->CheckDecision(
            this,
            context
        );
    }

    void FSMBrainCore::ApplyStateChange(
        FSMState* nextState,
        IFSMContext& context)
    {
        if (nextState == nullptr)
            return;

        // 이미 해당 상태가 정상적으로 실행 중
        if (mActiveState == nextState &&  mbStarted)
        {
            return;
        }

        if (mActiveState != nullptr && mbStarted)
        {
            mActiveState->ExitState(
                this,
                context
            );
        }

        mActiveState = nextState;
        mPendingState = nullptr;

        mStateElapsedTime = 0.0f;

        mActiveState->EnterState(
            this,
            context
        );

        mbStarted = true;
    }

    void FSMBrainCore::ChangeState(FSMState* nextState)
    {
        if (nextState == nullptr)
            return;

        // 현재 Update가 실행 중이 아님
        // 즉 사용할 수 있는 Context가 없음
        if (mCurrentContext == nullptr)
        {
            if (mActiveState == nextState &&
                mbStarted)
            {
                return;
            }

            // 지금 Enter/Exit를 호출하지 않고
            // 다음 Update의 유효한 Context를 기다린다
            mPendingState = nextState;
            return;
        }

        // Update 실행 중에는 Context가 유효하므로
        // 즉시 상태 전환 가능
        ApplyStateChange(
            nextState,
            *mCurrentContext
        );

    
    }

    void FSMBrainCore::SendFSMEvent(const std::string& eventName)
    {
        FSMState* nextState = FindState(eventName);

        if (nextState == nullptr)
            return;

        ChangeState(nextState);
    }

    void FSMBrainCore::AddState( const std::string& name, std::unique_ptr<FSMState> state)
    {
        if (state == nullptr)
            return;

        mStates[name] = std::move(state);
    }

    bool FSMBrainCore::SetInitialState(const std::string& name)
    {
        FSMState* initialState =
            FindState(name);

        if (initialState == nullptr)
            return false;

        mActiveState = initialState;
        mPendingState = nullptr;
        mCurrentContext = nullptr;

        mStateElapsedTime = 0.0f;
        mbStarted = false;

        return true;
    }

    FSMState* FSMBrainCore::FindState(const std::string& name) const
    {
        auto iter = mStates.find(name);

        if (iter == mStates.end())
            return nullptr;

        return iter->second.get();
    }

    FSMState* FSMBrainCore::GetActiveState() const
    {
        return mActiveState;
    }

    std::string FSMBrainCore::GetActiveStateName() const
    {
        if (mActiveState == nullptr)
            return {};

        return mActiveState->GetStateName();
    }

    float FSMBrainCore::GetStateElapsedTime() const
    {
        return mStateElapsedTime;
    }

    BlackBoard* FSMBrainCore::GetBlackboard() const
    {
        return mBlackboard.get();
    }

    void FSMBrainCore::Clear()
    {
        if (mActiveState != nullptr &&
            mCurrentContext != nullptr &&
            mbStarted)
        {
            mActiveState->ExitState(
                this,
                *mCurrentContext
            );
        }

        mStates.clear();

        mActiveState = nullptr;
        mPendingState = nullptr;
        mCurrentContext = nullptr;

        mStateElapsedTime = 0.0f;
        mbStarted = false;

        mBlackboard =
            std::make_unique<BlackBoard>();
    }
}
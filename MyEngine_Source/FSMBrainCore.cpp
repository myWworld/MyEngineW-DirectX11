#include "FSMBrainCore.h"

#include "IFSMContext.h"
#include "MEBlackBoard.h"

#include <algorithm>

namespace ME
{
    FSMBrainCore::FSMBrainCore()
        : mBlackboard(std::make_unique<BlackBoard>())
    {
    }

    FSMBrainCore::~FSMBrainCore() = default;

    void FSMBrainCore::Update(IFSMContext& context)
    {
        if (mActiveState == nullptr)
            return;

        mCurrentContext = &context;

        if (!mbStarted)
        {
            mStateElapsedTime = 0.0f;
            mActiveState->EnterState(this, context);
            mbStarted = true;
        }

        mStateElapsedTime += std::max(0.0f, context.GetDeltaTime());

        // 현재 상태의 Task 실행
        mActiveState->UpdateTask(this, context);

        // Task 또는 외부 이벤트에서 상태가 제거될 가능성 방어
        if (mActiveState != nullptr)
        {
            mActiveState->CheckDecision(this, context);
        }
    }

    void FSMBrainCore::ChangeState(FSMState* nextState)
    {
        if (nextState == nullptr)
            return;

        if (mActiveState == nextState)
            return;

        // 아직 첫 Update 전이면 Context가 없으므로
        // 상태 포인터만 바꾸고 첫 Update에서 Enter한다
        if (mCurrentContext == nullptr)
        {
            mActiveState = nextState;
            mStateElapsedTime = 0.0f;
            mbStarted = false;
            return;
        }

        if (mActiveState != nullptr && mbStarted)
        {
            mActiveState->ExitState(
                this,
                *mCurrentContext
            );
        }

        mActiveState = nextState;
        mStateElapsedTime = 0.0f;

        mActiveState->EnterState(
            this,
            *mCurrentContext
        );

        mbStarted = true;
    }

    void FSMBrainCore::SendFSMEvent(const std::string& eventName)
    {
        FSMState* nextState =
            FindState(eventName);

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
        mCurrentContext = nullptr;

        mStateElapsedTime = 0.0f;
        mbStarted = false;

        mBlackboard =
            std::make_unique<BlackBoard>();
    }
}
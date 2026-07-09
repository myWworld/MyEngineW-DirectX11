#include "MEFSMBrain.h"
#include "MEFSMState.h"
#include "METime.h"
#include "MEBlackBoard.h"
#include "../MyEngine_Source/MENetworkManager.h"
#include "../MyEngine_Source/Protocol.h"

namespace ME
{
	FSMBrain::FSMBrain() :
		Component(enums::eComponentType::FSMBrain)
		, mActiveState(nullptr)
		, mOwner(nullptr)
	{
		mBlackboard = std::make_unique<BlackBoard>();
	}
	FSMBrain::~FSMBrain()
	{

	}

	void FSMBrain::Initialize()
	{
		mOwner = this->GetOwner();
	}
	void FSMBrain::Update()
	{
		if(mOwner == nullptr) 
			mOwner = this->GetOwner();


		if (mActiveState == nullptr) return;

		if (ME::NetworkManager::IsHost() == false)
		{
			// FSM 실행 안 함.
			// Transform 업데이트 안 함.
			// 오직 네트워크 패킷 수신부에서 강제로 덮어씌우는 위치와 애니메이션만 렌더링됨.
			return;
		}

		mActiveState->UpdateTask(this, mOwner);
		mActiveState->CheckDecision(this, mOwner);

		
	}
	
	void FSMBrain::LateUpdate()
	{

	}

	void FSMBrain::Render()
	{

	}

	void FSMBrain::AddState(const std::string& name, std::unique_ptr<FSMState> state)
	{
		mStates[name] = std::move(state);
	}

	void FSMBrain::SetInitialState(const std::string& name)
	{
		mActiveState = mStates[name].get();

		if (mActiveState)
		{
			mActiveState->EnterState(this, GetOwner());
		}
	}

	void FSMBrain::ChangeState(FSMState* nextState)
	{
		if (nextState == nullptr)return;
		if (mActiveState == nextState) return;

	//	OutputDebugStringA("[FSM] 상태가 변경되었습니다!!!\n");

		if (mActiveState != nullptr)
		{
			mActiveState->ExitState(this, mOwner);
		}

		mActiveState = nextState;

		if (mActiveState != nullptr)
		{
			mActiveState->EnterState(this, mOwner);
		}
	}

	void FSMBrain::SendFSMEvent(std::string eventName)
	{
		if (mStates.find(eventName) == mStates.end())
		{
			OutputDebugStringA("[FSM] 이벤트 이름이 존재하지 않습니다!!!\n");
			return;
		}
		FSMState* nextState = mStates[eventName].get();
		ChangeState(nextState);
	}
}
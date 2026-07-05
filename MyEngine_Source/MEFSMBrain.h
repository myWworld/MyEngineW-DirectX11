#pragma once
#include "MEComponent.h"
#include <unordered_map>

namespace ME
{

	class FSMState;
	class BlackBoard;
	class FSMBrain: public Component
	{

	public:
		FSMBrain();
		~FSMBrain();

		void Initialize()override;
		void Update()override;
		void LateUpdate()override;
		void Render() override;
		
		BlackBoard* GetBlackboard() { return mBlackboard.get(); }

		void ChangeState(FSMState* nextState);

		void SendFSMEvent(std::string eventName);

		void AddState(const std::string& name, std::unique_ptr<FSMState> state);

		void SetInitialState(const std::string& name);

	public:

		GameObject* mOwner;
		std::unique_ptr<BlackBoard> mBlackboard;
		std::unordered_map<std::string, std::unique_ptr<FSMState>> mStates;
		FSMState* mActiveState;

	};


}


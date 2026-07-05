#pragma once
#include <vector>
#include <string>
#include <memory>

namespace ME
{
	class FSMDecision;
	class FSMTask;
	class FSMBrain;
	class GameObject;
	class FSMState;

	struct FSMTransition
	{
		std::unique_ptr<FSMDecision> Decision;
		FSMState* TrueState;
		FSMState* FalseState;

		FSMTransition(std::unique_ptr<FSMDecision> decision, FSMState* trueState, FSMState* falseState = nullptr);

		FSMTransition(FSMTransition&& other) noexcept;

	};


	class FSMState
	{

	public:


		FSMState();
		~FSMState();

		void EnterState(FSMBrain* brain,  GameObject* owner);
		void ExitState(FSMBrain* brain, GameObject* owner);
		void UpdateTask(FSMBrain* brain,GameObject* owner);
		void CheckDecision(FSMBrain* brain, GameObject* owner);

		void AddTransition(FSMTransition&& transition);

		void AddTransition(std::unique_ptr<FSMDecision> decision, FSMState* trueState, FSMState* falseState);

		void AddTask(std::unique_ptr<FSMTask> task);
		void SetStateName(const std::string& name) { mStateName = name; }
		std::string GetStateName() const { return mStateName; }

	public:


		std::vector<FSMTransition> mTransitions;
		std::vector<std::unique_ptr<FSMTask>> mTasks;
		std::string mStateName;

	};


}


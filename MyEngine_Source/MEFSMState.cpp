#include "MEFSMState.h"
#include "MEFSMTask.h"
#include "MEFSMDecision.h"
#include "MEFSMBrain.h"
#include "FSMBrainCore.h"
#include "IFSMContext.h"

namespace ME
{

	FSMState::FSMState() = default;
	FSMState::~FSMState() = default;

	FSMTransition::FSMTransition(std::unique_ptr<FSMDecision> decision, FSMState* trueState, FSMState* falseState)
		: Decision(std::move(decision))
		, TrueState(trueState)
		, FalseState(falseState)
	{
	}

	FSMTransition::FSMTransition(FSMTransition&& other) noexcept
		: Decision(std::move(other.Decision))
		, TrueState(other.TrueState)
		, FalseState(other.FalseState)
	{
	}

	void FSMState::EnterState(
		FSMBrainCore* brain,
		IFSMContext& context)
	{
		for (const auto& task : mTasks)
		{
			task->Enter(brain, context);
		}
	}

	void FSMState::UpdateTask(
		FSMBrainCore* brain,
		IFSMContext& context)
	{
		for (const auto& task : mTasks)
		{
			task->Execute(brain, context);
		}
	}

	void FSMState::UpdateTask(FSMBrainCore* brain,IFSMContext& context)
	{
		for (const auto& task : mTasks)
		{
			task->Execute(brain, context);
		}
	}

	void FSMState::CheckDecision(FSMBrainCore* brain, IFSMContext& context)
	{
		for (const auto& transition : mTransitions)
		{
			auto decisionState = transition.Decision->Decision(brain ,owner);
			
			if (decisionState == eDecisionResult::True && transition.TrueState != nullptr)
			{
				brain->ChangeState(transition.TrueState);
				break;
			}
			else if(decisionState == eDecisionResult::False && transition.FalseState != nullptr)
			{
				brain->ChangeState(transition.FalseState);
				break;
			}
		}
	}

	void FSMState::AddTransition(std::unique_ptr<FSMDecision> decision, FSMState* trueState, FSMState* falseState)
	{
		mTransitions.emplace_back(std::move(decision), trueState, falseState);
	}

	void FSMState::AddTask(std::unique_ptr<FSMTask> task)
	{
		mTasks.push_back(std::move(task));
	}

	void FSMState::AddTransition(FSMTransition&& transition)
	{
		mTransitions.push_back(std::move(transition));
	}

}
#pragma once
#include <unordered_map>
#include <string>
#include "json.hpp"
#include "MEFSMDecision.h"
#include "MEFSMState.h"
#include "MEFSMTask.h"
#include "MEFSMBrain.h"
#include "../MyEngine_W/MEAnimFinishDecision.h"
#include "../MyEngine_W/MEDetectTargetDecision.h"
#include "../MyEngine_W/MEDistanceDecision.h"
#include "../MyEngine_W/METimerDecision.h"
#include "../MyEngine_W/MEMoveToTargetTask.h"
#include "../MyEngine_W/MEPlayAnimTask.h"
#include "../MyEngine_W/MERandomWanderTask.h"
#include "../MyEngine_W/MEDestroyTask.h"
#include "../MyEngine_W/MEMeleeAttackTask.h"

#define REGISTER_TASK(type) mTaskCreators[#type] = [] () {return std::make_unique<type>();}
#define REGISTER_DECISION(type) mDecisionCreators[#type] = [] () {return std::make_unique<type>();}

namespace ME
{
	static class FSMFactory
	{

		using CreateTask = std::function<std::unique_ptr<FSMTask>()>;
		using CreateDecision= std::function<std::unique_ptr<FSMDecision>()>;

	public:

		inline static std::unordered_map<std::string, CreateTask> mTaskCreators;
		inline static std::unordered_map<std::string, CreateDecision> mDecisionCreators;
		

		static void Initialize()
		{
			REGISTER_TASK(PlayAnimTask);
			REGISTER_TASK(RandomWanderTask);
			REGISTER_TASK(MoveToTargetTask);
			REGISTER_TASK(MeleeAttackTask);
			REGISTER_TASK(DestroyTask);

			REGISTER_DECISION(TimerDecision);
			REGISTER_DECISION(AnimFinishDecision);
			REGISTER_DECISION(DetectTargetDecision);
			REGISTER_DECISION(DistanceDecision);
		}


		static std::unique_ptr<FSMTask> GetTask(const std::string& name)
		{
			auto iter = mTaskCreators.find(name);

			if (iter != mTaskCreators.end())
			{
				return iter->second();
			}

			return nullptr;
		}

		static std::unique_ptr<FSMDecision> GetDecision(const std::string& name)
		{
			auto it = mDecisionCreators.find(name);
			if (it != mDecisionCreators.end())
			{
				return it->second();
			}
			return nullptr;
		}


		static void MakeFSMWithJsonFile(FSMBrain* brain, const std::string& jsonPath)
		{
		
			std::ifstream file(jsonPath);
			if (file.is_open())
			{
				nlohmann::json datas = nlohmann::json::parse(file);

				for (const auto& stateJson : datas["States"])
				{
					std::string stateName = stateJson["Name"];
					auto newState = std::make_unique<FSMState>();

					if (newState)
					{
						newState->SetStateName(stateName);
						brain->AddState(stateName, std::move(newState));
					}
				}

				for (const auto& stateJson : datas["States"])
				{
					auto stateName = stateJson["Name"];
					
					for (const auto& taskJson : stateJson["Tasks"])
					{
						std::string type = taskJson["Type"];
						auto task = GetTask(type);

						if (task)
						{
							task->LoadFromJson(taskJson);
							brain->mStates[stateName]->AddTask(std::move(task));
						}

					}

					for (const auto& transitionsJson : stateJson["Transitions"])
					{
						auto decisionJson = transitionsJson["Decision"];

						std::string decisionType = decisionJson["Type"];
						auto decision = GetDecision(decisionType);

						if (decision)
						{
							decision->LoadFromJson(decisionJson);

		

							FSMState* trueState = nullptr;
							if (transitionsJson.contains("TrueState") && !transitionsJson["TrueState"].is_null())
							{
								std::string trueStateName = transitionsJson["TrueState"];
								trueState = brain->mStates[trueStateName].get();
							}

							FSMState* falseState = nullptr;
							if (transitionsJson.contains("FalseState") && !transitionsJson["FalseState"].is_null())
							{
								std::string falseStateName = transitionsJson["FalseState"];
								falseState = brain->mStates[falseStateName].get();
							}

							brain->mStates[stateName]->AddTransition(std::move(decision), trueState, falseState);

						}

					}
				
				}

				if (datas.contains("InitialState"))
				{
					std::string initial = datas["InitialState"];
					brain->SetInitialState(initial); 
				}




			}
		
		}


	};
}
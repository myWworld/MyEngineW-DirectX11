#pragma once
#include "METime.h"
#include "json.hpp"
#include <unordered_map>
#include <functional>
#include <string>

namespace ME
{
	class FSMBrain;
	class GameObject;

	enum class eDecisionResult
	{
		True,
		False,
		Pending,
		End,
	};

	class FSMDecision
	{
	private:
		std::unordered_map<std::string, std::function<void(const nlohmann::json&)>> mProperties;

	public:

		FSMDecision():mTimer(0.0f) , mCheckInterval(0.2f){}
		virtual ~FSMDecision() = default;

		virtual void LoadFromJson(const nlohmann::json& j) final
		{
			// 등록된 모든 프로퍼티를 순회하며
			for (auto& [key, setterFunc] : mProperties)
			{
				// JSON에 그 이름이 있으면 람다 함수 실행
				if (j.contains(key))
				{
					setterFunc(j[key]);
				}
			}
		}

		eDecisionResult Decision(FSMBrain* brain, GameObject* owner)
		{
			mTimer += Time::DeltaTime();


			if (mTimer >= mCheckInterval)
			{
				mTimer = 0.0f;
				return  CheckDecision(brain, owner) ? eDecisionResult::True : eDecisionResult::False;	
			}

			return eDecisionResult::Pending;
		}
	
		void SetIntervalTime(float intervalTime) { mCheckInterval = intervalTime; }



	protected:

		virtual bool CheckDecision(FSMBrain* brain, GameObject* owner) = 0;
		
		template<typename T>
		void BindProperty(const std::string& name, T* memberPtr) //json파일에 있는 Decision별 멤버 변수, 자식에게서 포인터로 받아들인뒤 나중에 람다함수 실행
		{
			mProperties[name] = [memberPtr](const nlohmann::json& j)
				{
					*memberPtr = j.get<T>();
				};
		}

	protected:

		float mTimer;
		float mCheckInterval;

	};
}

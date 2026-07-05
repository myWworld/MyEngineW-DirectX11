#pragma once
#include "json.hpp"

#include <unordered_map>
#include <functional>
#include <string>

#define BIND_PROP(var) BindProperty(#var, &var) 



namespace ME
{
	class GameObject;
	class FSMBrain;
	class FSMTask
	{
	private :

		std::unordered_map<std::string, std::function<void(const nlohmann::json&)>> mProperties;

		public :
	
			FSMTask():bIsFinished(false),bIsLoop(false){}
			virtual ~FSMTask() = default;

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

			virtual void Enter(FSMBrain* brain, GameObject* owner)
			{
				bIsFinished = false; // 다시 이 상태로 돌아왔을 때 플래그를 무조건 초기화
				OnEnter(brain, owner); 
			}
			
			void Execute(FSMBrain* brain, GameObject* owner)
			{
				if (bIsFinished) return;

				OnExecute(brain, owner);

				if (!bIsLoop)
				{
					bIsFinished = true;
				}
			}

			virtual void Exit(FSMBrain* brain, GameObject* owner)
			{
				OnExit(brain, owner);     
			}

	protected:
		virtual void OnEnter(FSMBrain* brain, GameObject* owner) {}
		virtual void OnExecute(FSMBrain* brain, GameObject* owner) = 0;
		virtual void OnExit(FSMBrain* brain, GameObject* owner) {}

		template<typename T>
		void BindProperty(const std::string& name, T* memberPtr) //json파일에 있는 task별 멤버 변수, 자식에게서 포인터로 받아들인뒤 나중에 람다함수 실행
		{
			mProperties[name] = [memberPtr](const nlohmann::json& j)
				{
					*memberPtr = j.get<T>();
				};
		}

	public :

		bool bIsFinished;
		bool bIsLoop;
	};


}


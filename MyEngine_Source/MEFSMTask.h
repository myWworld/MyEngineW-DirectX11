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

			void Execute(FSMBrainCore* brain, IFSMContext& context)
			{
				if (bIsFinished)
					return;

				OnExecute(brain, context);

				if (!bIsLoop)
				{
					bIsFinished = true;
				}
			}

			void Exit(FSMBrainCore* brain, IFSMContext& context)
			{
				OnExit(brain, context);
			}

	protected:
		virtual void OnEnter(FSMBrainCore* brain, IFSMContext& context)
		{
		}

		virtual void OnExecute(FSMBrainCore* brain, IFSMContext& context) = 0;

		virtual void OnExit(FSMBrainCore* brain, IFSMContext& context)
		{
		}

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


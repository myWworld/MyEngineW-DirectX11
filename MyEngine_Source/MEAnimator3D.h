
#pragma once

#include "MEComponent.h"
#include "MEAnimation3D.h"
#include "CommonInclude.h"
#include "MESkeleton.h"

namespace ME
{


	class Animator3D :public Component
	{
	public:

		struct Event
		{
			void operator()()
			{
				if (mEvent)
					mEvent();
			}

			void operator=(std::function<void()>func)
			{
				mEvent = std::move(func);
			}

			std::function<void()> mEvent;
		};

		struct Events
		{
			Event StartEvent;
			Event CompleteEvent;
			Event EndEvent;
		};

		Animator3D();
		~Animator3D();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render() override;

		void Bind();

		void CreateAnimation(const std::wstring& name, const std::wstring& path);


		Animation3D* FindAnimation(const std::wstring& name);
		void PlayAnimation(const std::wstring& name, bool loop = true);

		Events* FindEvents(const std::wstring& name);

		std::function<void()>& GetStartEvent(const std::wstring& name);
		std::function<void()>& GetCompleteEvent(const std::wstring& name);
		std::function<void()>& GetEndEvent(const std::wstring& name);

		bool IsComplete() { return mActiveAnimation->IsComplete(); }
		void SetSkeleton(Skeleton* skeleton) { mSkeleton = skeleton; }
		Skeleton* GetSkeleton() { return mSkeleton; }


	private:

		std::map<std::wstring, Animation3D*> mAnimations;

		bool mbLoop;
		
		Animation3D* mActiveAnimation;
		Skeleton* mSkeleton;


		//Event
		std::map<std::wstring, Events*> mEvents;

	};


}





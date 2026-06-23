
#pragma once

#include "MEComponent.h"
#include "MEAnimation3D.h"
#include "CommonInclude.h"
#include "MESkeleton.h"


namespace ME
{
	class Material;
	class graphics::Texture;
	class Mesh;

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

		void SetSkeleton(Skeleton* skeleton) { 
			
			mSkeleton = *skeleton; 
			this->mModelType = mSkeleton.GetModelType();
		}
		Skeleton GetSkeleton() { return mSkeleton; }
		Skeleton* GetSkeletonPtr() { return &mSkeleton; }

		void SetApplyRootMotion(bool apply) { mbApplyRootMotion = apply; }
		bool GetApplyRootMotion() { return mbApplyRootMotion; }


		Vector3 GetPrevRootPos() { return mPrevRootPos; }
		void SetPrevRootPos(const Vector3& pos) { mPrevRootPos = pos; }


		bool IsAnimationComplete() { return mbComplete; }

	private:

		void boneTransformBind();
		void render();

	private:



		Matrix mModelMatrix;

		bool mbLoop;
		bool mbApplyRootMotion = false;

		float mCurrentTime;
		bool mbComplete;

		Animation3D* mActiveAnimation;
		Skeleton mSkeleton;

		math::Vector3 mPrevRootPos = Vector3::Zero;
		math::Vector3 mRootMotionTotalOffset = Vector3::Zero;
		math::Vector3 mRootMotionBasePosition = Vector3::Zero;

		std::unordered_map<std::wstring, Animation3D*> mAnimations;
		std::unordered_map<std::wstring, std::unique_ptr<Events>> mEvents;

		enums::eModelType mModelType;

	};

	extern std::unordered_map<std::string, std::string> BoneNameManualMapping;

}





#include "MEAnimator3D.h"
#include "MEResources.h"
#include "METexture.h"
#include "MEConstantBuffer.h"
#include "MERenderer.h"
#include "METransform.h"
#include "MEGameObject.h"
#include "MEMesh.h"
#include "MEMaterial.h"
#include "METime.h"


namespace ME
{
	Animator3D::Animator3D()
		:Component(enums::eComponentType::Animator)
		, mActiveAnimation(nullptr)
		, mAnimations{}
		, mbLoop(false)
		, mEvents{}
		, mModelType(enums::eModelType::StaticBone)
		, mSkeleton{}
		, mModelMatrix{}
		, mCurrentTime(0.0f)
		, mPrevRootPos(Vector3::Zero)
		, mRootMotionTotalOffset(Vector3::Zero)
		, mRootMotionBasePosition(Vector3::Zero)
	{
	}
	Animator3D::~Animator3D()
	{
		mAnimations.clear();
		mEvents.clear();
	}
	void Animator3D::Initialize()
	{
	}
	void Animator3D::Update()
	{
		if (mActiveAnimation)
		{

			mCurrentTime += Time::DeltaTime();

			
			float duration = mActiveAnimation->GetDuration();
			float ticksPerSec = mActiveAnimation->GetTickersPerSecond();

			if (mCurrentTime * ticksPerSec > duration)
			{
				mbComplete = true;
				if (mbLoop) {
					mCurrentTime = 0.0f; 
					mbComplete = false;
				}
			}

			mActiveAnimation->UpdateAnimation(mCurrentTime, &mSkeleton, this);

	
			if (mbComplete)
			{
				Events* events = FindEvents(mActiveAnimation->GetName());
				if (events) events->CompleteEvent();
			}

		}
	}

	void Animator3D::LateUpdate()
	{
	}

	void Animator3D::Render()
	{
		//if (mActiveAnimation)
		//{
		//	mActiveAnimation->Render(hdc);
		//}
		Bind();


	}

	void Animator3D::render()
	{
	
	

	}


	void Animator3D::Bind()
	{
		boneTransformBind();

		if (mModelType == enums::eModelType::SkinnedMesh)
		{
			graphics::AnimationCB cbData; // {} -> to none Zero-초기화를 매번하는 작업 스택에 할당되므로 오버헤드가 적음. 하지만 매번 초기화하는 작업이 필요함.
			// -> 지역캐시성을 높이기 위해 지역변수로 선언하는 것이 좋음.

			for (unsigned i = 0; i < mSkeleton.mBones.size(); i++)
			{
				cbData.BoneMatrices[i] = mSkeleton.mBones[i].FinalTransform;
		
			}

			graphics::ConstantBuffer* cb = renderer::constantBuffers[CBSLOT_ANIMATION];

			cb->SetData(&cbData);
			cb->Bind(graphics::eShaderStage::All);
		}

		
	
	}

	void Animator3D::boneTransformBind()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 position = tr->GetPosition();
		tr->Bind();

	}


	void Animator3D::CreateAnimation(const std::wstring& name, const std::wstring& path)
	{
		if (FindAnimation(name) != nullptr)
			return;

		std::shared_ptr<Animation3D> animation = Resources::Load<Animation3D>(name, path);

		auto events = std::make_unique<Events>();

		mEvents.insert({ name, std::move(events)});
		mAnimations.insert({ name, animation.get()});

	}




	Animation3D* Animator3D::FindAnimation(const std::wstring& name)
	{
		auto iter = mAnimations.find(name);
		if (iter == mAnimations.end())
			return nullptr;

		return iter->second;


	}
	void Animator3D::PlayAnimation(const std::wstring& name, bool loop)
 	{
		std::shared_ptr<Animation3D> animation = Resources::Find<Animation3D>(name);

		if (animation == nullptr)
			return;

		if (mActiveAnimation && mActiveAnimation == animation.get())
			return;

		if (mActiveAnimation)
		{
			Events* currentEvents
				= FindEvents(mActiveAnimation->GetName());

			if (currentEvents)
			{
				currentEvents->EndEvent();
			}
		}

		Events* nextEvents
			= FindEvents(animation->GetName());

		if (nextEvents)
		{
			nextEvents->StartEvent();
		}

		mActiveAnimation = animation.get();
		this->mPrevRootPos = Vector3::Zero;
		mbLoop = loop;

		mCurrentTime = 0.0f;
		mbComplete = false;

	}

	Animator3D::Events* Animator3D::FindEvents(const std::wstring& name)
	{
		auto iter = mEvents.find(name);
		if (iter == mEvents.end())
			return nullptr;

		return iter->second.get();
	}

	std::function<void()>& Animator3D::GetStartEvent(const std::wstring& name)
	{
		Events* events = FindEvents(name);
		return events->StartEvent.mEvent;

	}
	std::function<void()>& Animator3D::GetCompleteEvent(const std::wstring& name)
	{
		Events* events = FindEvents(name);
		return events->CompleteEvent.mEvent;
	}
	std::function<void()>& Animator3D::GetEndEvent(const std::wstring& name)
	{
		Events* events = FindEvents(name);
		return events->EndEvent.mEvent;
	}


	std::unordered_map<std::string, std::string> BoneNameManualMapping = {
	
		{"righttoe_end", "rightfoottoebase_end"},

	};
}
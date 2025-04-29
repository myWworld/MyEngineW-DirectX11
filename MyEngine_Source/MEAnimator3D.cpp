#include "MEAnimator3D.h"
#include "MEResources.h"
#include "METexture.h"
#include "MEConstantBuffer.h"
#include "MERenderer.h"
#include "METransform.h"
#include "MEGameObject.h"


namespace ME
{
	Animator3D::Animator3D()
		:Component(enums::eComponentType::Animator)
		, mActiveAnimation(nullptr)
		, mAnimations{}
		, mbLoop(false)
		, mEvents{}
		, mModelType(enums::eModelType::StaticBone)
	{
	}
	Animator3D::~Animator3D()
	{
		for (auto& iter : mAnimations)
		{
			delete iter.second;
			iter.second = nullptr;
		}

		for (auto& iter : mEvents)
		{
			delete iter.second;
			iter.second = nullptr;
		}
	}
	void Animator3D::Initialize()
	{
	}
	void Animator3D::Update()
	{
		if (mActiveAnimation)
		{
			mActiveAnimation->Update();

			Events* events = FindEvents(mActiveAnimation->GetName());


			if (mActiveAnimation->IsComplete() == true)
			{
				if (mbLoop == true)
					mActiveAnimation->Reset();

				if (events)
				{
					events->CompleteEvent();
				}

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

	void Animator3D::Bind()
	{
		

		if (mModelType == enums::eModelType::SkinnedMesh)
		{
			graphics::AnimationCB cbData = {};

			for (unsigned i = 0; i < mSkeleton->mBones.size(); i++)
			{
				cbData.BoneMatrices[i] = mSkeleton->mBones[i].FinalTransform;
			}

			graphics::ConstantBuffer* cb = renderer::constantBuffers[CBSLOT_ANIMATION];

			cb->SetData(&cbData);
			cb->Bind(graphics::eShaderStage::All);
		}
	
	}

	void Animator3D::boneTransformBind()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		tr->Bind();

		
	}


	void Animator3D::CreateAnimation(const std::wstring& name, const std::wstring& path)
	{
		Animation3D* animation = nullptr;

		animation = FindAnimation(name);

		if (animation != nullptr)
			return;

		animation = new Animation3D();
		animation->SetName(name);
		animation->SetAnimator(this);
		animation->CreateAnimation(name, path);

		Events* events = new Events();
		mEvents.insert({ name, events });

		mAnimations.insert({ name, animation });

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
		Animation3D* animation = FindAnimation(name);
		if (animation == nullptr)
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

		mActiveAnimation = animation;
		mActiveAnimation->Reset();
		mbLoop = loop;

	}

	Animator3D::Events* Animator3D::FindEvents(const std::wstring& name)
	{
		auto iter = mEvents.find(name);
		if (iter == mEvents.end())
			return nullptr;

		return iter->second;
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
	{"hips", "pelvis"},
	{"spine", "spine"},
	{"spine1", "spine"},    // spine1, spine2는 spine으로 fallback
	{"spine2", "spine"},
	{"neck", "neck"},
	{"head", "head"},

	{"leftarm", "upper_arm_left"},
	{"leftforearm", "forearm_left"},
	{"lefthand", "hand_left"},

	{"rightarm", "upper_arm_right"},
	{"rightforearm", "forearm_right"},
	{"righthand", "hand_right"},

	{"leftupleg", "thigh_left"},
	{"leftleg", "shin.l"},
	{"leftfoot", "foot_left"},

	{"rightupleg", "thigh_right"},
	{"rightleg", "shin.r"},
	{"rightfoot", "foot_right"},

	// 손가락 일부 매핑 (모델에 없는 경우 생략하거나 무시)
	{"lefthandthumb1", "thumb.01.l"},
	{"lefthandthumb2", "thumb.02.l"},
	{"righthandthumb1", "thumb.01.r"},
	{"righthandthumb2", "thumb.02.r"},

	{"lefthandmiddle1", "f_middle.01.l"},
	{"righthandmiddle1", "f_middle.01.r"},

	};
}
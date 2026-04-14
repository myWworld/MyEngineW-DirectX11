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
		, mMaterial(nullptr)
		, mMeshes{}
		, mTextures{}
		, mModelMatrix{}
		, mStaticMaterial(nullptr)
		, mCurrentTime(0.0f)
	{
	}
	Animator3D::~Animator3D()
	{
		mAnimations.clear();
		mEvents.clear();
	}
	void Animator3D::Initialize()
	{
		mMaterial = Resources::Find<Material>(L"ModelMaterial").get();
		mStaticMaterial = Resources::Find<Material>(L"StaticModelMaterial").get();
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
		render();

	}

	void Animator3D::render()
	{
		Bind();


		if (!mMeshes.empty())
		{
			for (auto mesh : mMeshes)
			{
				if (mesh == nullptr)
					continue;

				if (mesh->IsSkinned() == true)
				{		
					mMaterial->BindShader();
				}
				else
				{				
					mStaticMaterial->BindShader();
				}

				mesh->Bind();

				if (mesh->GetDiffuseTexture() != nullptr)
				{
					mesh->GetDiffuseTexture()->Bind(graphics::eShaderStage::PS, (UINT)graphics::eTextureType::Albedo);
				}

				if (mesh->GetSpecularTexture() != nullptr)
				{
					mesh->GetSpecularTexture()->Bind(graphics::eShaderStage::PS, (UINT)graphics::eTextureType::Specular);
				}

				if (mesh->GetNormalTexture() != nullptr)
				{
					mesh->GetNormalTexture()->Bind(graphics::eShaderStage::PS, (UINT)graphics::eTextureType::Normal);
				}

				if (!mesh->GetSpecularTexture() && !mesh->GetDiffuseTexture() && !mTextures.empty())
				{
					for (auto texture : mTextures)
					{
						if (texture)
							texture->Bind(graphics::eShaderStage::PS, (UINT)graphics::eTextureType::Albedo);
					}
				}

				graphics::GetDevice()->DrawIndexed(mesh->GetIndexCount(), 0, 0);
			
			}
		}

	}


	void Animator3D::Bind()
	{
		boneTransformBind();

		if (mModelType == enums::eModelType::SkinnedMesh)
		{
			graphics::AnimationCB cbData = {};

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

		auto animation = std::make_unique<Animation3D>();
		animation->SetName(name);
		animation->SetAnimator(this);
		animation->CreateAnimation(name, path);

		auto events = std::make_unique<Events>();

		mEvents.insert({ name, std::move(events)});
		mAnimations.insert({ name, std::move(animation)});

	}




	Animation3D* Animator3D::FindAnimation(const std::wstring& name)
	{
		auto iter = mAnimations.find(name);
		if (iter == mAnimations.end())
			return nullptr;

		return iter->second.get();


	}
	void Animator3D::PlayAnimation(const std::wstring& name, bool loop)
 	{
		Animation3D* animation = FindAnimation(name);
		if (animation == nullptr)
			return;

		if (mActiveAnimation && mActiveAnimation == animation)
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
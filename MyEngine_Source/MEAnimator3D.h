
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

		//void setModelMatrix(const Matrix& model);
		void SetMesh(const std::vector<Mesh*>& meshes_) { mMeshes = meshes_; }

		void SetTextures(const std::vector<graphics::Texture*>& textures) { mTextures = textures; }
		void SetTexture(graphics::Texture* texture) { mTextures.push_back(texture); }
		void SetTexture(std::shared_ptr<graphics::Texture> texture) { mTextures.push_back(texture.get()); }

		void SetMaterial(Material* material) { mMaterial = material; }

		void SetApplyRootMotion(bool apply) { mbApplyRootMotion = apply; }
		bool GetApplyRootMotion() { return mbApplyRootMotion; }


		Vector3 GetPrevRootPos() { return mPrevRootPos; }
		void SetPrevRootPos(const Vector3& pos) { mPrevRootPos = pos; }


	private:

		void boneTransformBind();
		void render();

	private:



		Matrix mModelMatrix;

		std::vector<Mesh*> mMeshes;
		std::vector<graphics::Texture* >mTextures;
		Material* mMaterial;
		Material* mStaticMaterial;

		bool mbLoop;
		bool mbApplyRootMotion = false;

		float mCurrentTime;
		bool mbComplete;

		math::Vector3 mPrevRootPos;

		Animation3D* mActiveAnimation;
		Skeleton mSkeleton;



		std::unordered_map<std::wstring, std::unique_ptr<Animation3D>> mAnimations;
		std::unordered_map<std::wstring, std::unique_ptr<Events>> mEvents;

		enums::eModelType mModelType;

	};

	extern std::unordered_map<std::string, std::string> BoneNameManualMapping;

}





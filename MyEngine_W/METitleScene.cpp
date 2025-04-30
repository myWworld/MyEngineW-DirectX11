#include "METitleScene.h"
#include "MEInput.h"
#include "MESceneManager.h"
#include "MEObject.h"
#include "MERenderer.h"
#include "MESpriteRenderer.h"
#include "METexture.h"
#include "MEResource.h"
#include "MEResources.h"
#include "METransform.h"
#include "MEApplication.h"

#include "MEGraphicDevice_DX11.h"
#include "MEUIManager.h"
#include "MEPlayer.h"
#include "MEModel.h"

#include "MECameraScript.h"
#include "MEModelRenderer.h"
#include "MEAnimator3D.h"

extern ME::Application application;

namespace ME
{


	ME::TitleScene::TitleScene()
	{
	}

	ME::TitleScene::~TitleScene()
	{
		
	}

	void ME::TitleScene::Initialize()
	{

		Scene::Initialize();

		GameObject* camera = object::Instantiate<GameObject>(enums::eLayerType::None, Vector3(0,0,0));

		Camera* cameraComp = camera->AddComponent<Camera>();
		cameraComp->SetProjectionMatrix(Camera::eProjectionType::Perspective);
		cameraComp->SetSize(1000.0f);

		CameraScript* cameraScript = camera->AddComponent<CameraScript>();
		renderer::mainCamera = cameraComp;

		
	}

	void ME::TitleScene::Update()
	{
		
		Scene::Update();
	}

	void ME::TitleScene::LateUpdate()
	{
		Scene::LateUpdate();

	}

	void ME::TitleScene::Render()
	{
 		Scene::Render();
	}

	void TitleScene::OnEnter()
	{
		if (!mPlayer)
		{

			mPlayer = object::Instantiate<Player>(enums::eLayerType::Player, Vector3(0, 0, 0));
			Transform* tr = mPlayer->GetComponent<Transform>();
			tr->SetScale(Vector3(3.f, 3.f, 3.f));
			//SpriteRenderer* sr = mPlayer->AddComponent<SpriteRenderer>();
			//sr->SetMaterial(Resources::Find<Material>(L"SpriteMaterial"));
			//sr->SetSprite(Resources::Find<graphics::Texture>(L"TITLE"));
			//
			Model* model = new Model();
			if (model->LoadModel(L"..\\Resources\\Soldier.fbx"))
			{

				//ModelRenderer* modelRenderer = mPlayer->AddComponent<ModelRenderer>();
				//modelRenderer->SetMesh(model->GetMeshes());
				//modelRenderer->SetMaterial(Resources::Find<Material>(L"StaticModelMaterial"));
				//modelRenderer->SetTextures(model->GetTextures());
				//modelRenderer->SetTexture(Resources::Find<graphics::Texture>(L"SOLDIER"));


				Animator3D* animator = mPlayer->AddComponent<Animator3D>();
				animator->SetMesh(model->GetMeshes());
				
				if(model->GetTextures().size() > 0)
					animator->SetTextures(model->GetTextures());
				else 
					animator->SetTexture( Resources::Find<graphics::Texture>(L"SOLDIER"));
				
				
				animator->SetSkeleton(&model->GetSkeleton());
				animator->CreateAnimation(L"Idle", L"..\\Resources\\Animation\\Idle.fbx");
				animator->PlayAnimation(L"Idle", true);
			}
			else
			{
				delete model;
				model = nullptr;
			}

		}



		UIManager::Push(enums::eUIType::Button);
		
		Scene::OnEnter();

	}

	void TitleScene::OnExit()
	{
		UIManager::Pop(enums::eUIType::Button);

		Scene::OnExit();

	}


}
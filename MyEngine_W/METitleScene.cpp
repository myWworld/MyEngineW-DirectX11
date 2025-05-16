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

#include "MEPlayerScript.h"
#include "MEGunScript.h"
#include "MEGun.h"

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

		GameObject* camera = object::Instantiate<GameObject>(enums::eLayerType::None, Vector3(0, 0, 0));

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
			tr->SetScale(Vector3(0.2f, 0.2f, 0.2f));
			renderer::mainCamera->SetTarget(mPlayer);

			//SpriteRenderer* sr = mPlayer->AddComponent<SpriteRenderer>();
			//sr->SetMaterial(Resources::Find<Material>(L"SpriteMaterial"));
			//sr->SetSprite(Resources::Find<graphics::Texture>(L"TITLE"));
			//
			mPlayer->AddComponent<PlayerScript>();
			Model* model = new Model();
			MakeCharacter(model);

			MakeGun();
		}



		UIManager::Push(enums::eUIType::Button);

		Scene::OnEnter();

	}

	void TitleScene::OnExit()
	{
		UIManager::Pop(enums::eUIType::Button);

		Scene::OnExit();

	}

	void TitleScene::MakeCharacter(Model* model)
	{
		if (model->LoadModel(L"..\\Resources\\alien.fbx"))
		{

			//ModelRenderer* modelRenderer = mPlayer->AddComponent<ModelRenderer>();
			//modelRenderer->SetMesh(model->GetMeshes());
			//modelRenderer->SetMaterial(Resources::Find<Material>(L"StaticModelMaterial"));
			//modelRenderer->SetTextures(model->GetTextures());
			//modelRenderer->SetTexture(Resources::Find<graphics::Texture>(L"SOLDIER"));

			Animator3D* animator = mPlayer->AddComponent<Animator3D>();
			animator->SetMesh(model->GetMeshes());

			if (model->GetTextures().size() > 0)
				animator->SetTextures(model->GetTextures());
			else
				animator->SetTexture(Resources::Find<graphics::Texture>(L"ALIEN"));


			animator->SetSkeleton(&model->GetSkeleton());
			animator->CreateAnimation(L"Idle", L"..\\Resources\\Animation\\Idle2.fbx");
			animator->CreateAnimation(L"FORWARDWALK", L"..\\Resources\\Animation\\ForwardWalk.fbx");
			animator->CreateAnimation(L"PISTOLWALK", L"..\\Resources\\Animation\\PistolWalk.fbx");
			animator->CreateAnimation(L"PISTOLIDLE", L"..\\Resources\\Animation\\PistolIdle.fbx");
			animator->CreateAnimation(L"PISTOLIDLE2", L"..\\Resources\\Animation\\PistolIdle2.fbx");
			animator->CreateAnimation(L"DANCE", L"..\\Resources\\Animation\\SillyDancing.fbx");
			animator->PlayAnimation(L"PISTOLIDLE", true);
		}
		else
		{
			delete model;
			model = nullptr;
		}
	}

	void TitleScene::MakeGun()
	{
		Model* gun = new Model();

		Gun* m4 = object::Instantiate<Gun>(enums::eLayerType::Items, Vector3(0, 10, 35));
		Transform* tr = m4->GetComponent<Transform>();
		tr->SetScale(Vector3(2.0f, 2.0f, 2.0f));
		GunScript* gunScript = m4->AddComponent<GunScript>();
		gunScript->SetGunOnwer(static_cast<Player*>(mPlayer));
	

		if (gun->LoadModel(L"..\\Resources\\beretta m9\\beretta m9.fbx"))
		{

			ModelRenderer* modelRenderer = m4->AddComponent<ModelRenderer>();
			modelRenderer->SetMesh(gun->GetMeshes());
			modelRenderer->SetMaterial(Resources::Find<Material>(L"StaticModelMaterial"));
			
			if (gun->GetTextures().size() > 0)
				modelRenderer->SetTextures(gun->GetTextures());
			else
				modelRenderer->SetTexture(Resources::Find<graphics::Texture>(L"PISTOL"));

			
		}
		else
		{
			delete gun;
			gun = nullptr;
		}
	}
}
		



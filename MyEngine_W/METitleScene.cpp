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

#include "MEUIManager.h"

#include "MECameraScript.h"
#include "MEModelRenderer.h"
#include "MEAnimator3D.h"
#include "MERigidbody.h"
#include "MEBoxCollider3D.h"

#include "MEPlayerScript.h"
#include "MEEnemyScript.h"
#include "MEGunScript.h"
#include "MEGun.h"
#include "MECollisionManager.h"

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

		CollisionManager::CollisionLayerCheck(enums::eLayerType::Player, enums::eLayerType::Bullet, true);
		CollisionManager::CollisionLayerCheck(enums::eLayerType::Bullet, enums::eLayerType::Player, true);

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

		
			GameObject* camera = object::Instantiate<GameObject>(enums::eLayerType::None, Vector3(0, 0, 0));

			Camera* cameraComp = camera->AddComponent<Camera>();
			cameraComp->SetProjectionMatrix(Camera::eProjectionType::Perspective);
			cameraComp->SetSize(1000.0f);

			CameraScript* cameraScript = camera->AddComponent<CameraScript>();
			renderer::mainCamera = cameraComp;
		
		if (!mPlayer)
		{

			mPlayer = object::Instantiate<Player>(enums::eLayerType::Player, Vector3(0, 0, 0));
			Transform* tr = mPlayer->GetComponent<Transform>();
			//tr->SetScale(Vector3(0.2f, 0.2f, 0.2f));
			renderer::mainCamera->SetTarget(mPlayer);

			//SpriteRenderer* sr = mPlayer->AddComponent<SpriteRenderer>();
			//sr->SetMaterial(Resources::Find<Material>(L"SpriteMaterial"));
			//sr->SetSprite(Resources::Find<graphics::Texture>(L"TITLE"));
			//
			mPlayer->AddComponent<PlayerScript>();
			MakeCharacter(mPlayer, L"AlienModel");

			MakeGun(mPlayer);

			GameObject* enemy = object::Instantiate<Player>(enums::eLayerType::Player, Vector3(10, 0, 0));
			Transform* transform = enemy->GetComponent<Transform>();
			//tr->SetScale(Vector3(0.2f, 0.2f, 0.2f));

			EnemyScript* enemyScript = enemy->AddComponent<EnemyScript>();
			MakeCharacter(enemy, L"AlienModel");

			MakeGun(enemy);
		}



		UIManager::Push(enums::eUIType::Button);

		Scene::OnEnter();

	}

	void TitleScene::OnExit()
	{
		if (mPlayer)
		{
			object::Destroy(mPlayer);
			mPlayer = nullptr;
		}


		if (renderer::mainCamera != nullptr)
		{
			GameObject* obj = renderer::mainCamera->GetOwner();
			object::Destroy(obj);
			renderer::mainCamera = nullptr;
		}

		UIManager::Pop(enums::eUIType::Button);

		Scene::OnExit();

	}

	void TitleScene::MakeCharacter(GameObject* player, std::wstring_view modelName)
	{
		std::shared_ptr<Model> model = Resources::Find<Model>(std::wstring(modelName));

		if (model)
		{


			auto* animator = player->AddComponent<Animator3D>();

			auto* modelRenderer = player->AddComponent<ModelRenderer>();
			modelRenderer->SetMesh(model->GetMeshes());
		
			
			player->SetModel(model);

		//	Rigidbody* rb = mPlayer->AddComponent<Rigidbody>();
			Collider* col = player->AddComponent<BoxCollider3D>();
			col->SetSize(Vector3(50, 250, 50));


			UIManager::Push(enums::eUIType::HpBar);


			animator->SetSkeleton(&model->GetSkeleton());
	
			animator->PlayAnimation(L"PISTOLIDLE", true);
		}
		else
		{
			model = nullptr;
		}
	}

	void TitleScene::MakeGun(GameObject* player  )
	{
		

		Gun* m4 = object::Instantiate<Gun>(enums::eLayerType::Items, Vector3(0, 10, 35));
		Transform* tr = m4->GetComponent<Transform>();
		//tr->SetScale(Vector3(7.0f, 7.0f, 7.0f));
	//	tr->SetRotation(Vector3(0.0f, 180.0f, 0.0f));
		GunScript* gunScript = m4->AddComponent<GunScript>();

		
		gunScript->SetOnwer(player);
	
		std::shared_ptr<Model> gun = Resources::Find<Model>(L"PistolModel");

		if (gun)
		{

			ModelRenderer* modelRenderer = m4->AddComponent<ModelRenderer>();
			modelRenderer->SetMesh(gun->GetMeshes());

			
		}
		else
		{
	
			gun = nullptr;
		}
	}
}
		



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
		if (!mPlayer)
		{
			mPlayer = object::Instantiate<Player>(enums::eLayerType::BackGround, Vector3(0, 0, 0));

			SpriteRenderer* sr = mPlayer->AddComponent<SpriteRenderer>();
			sr->SetMaterial(Resources::Find<Material>(L"SpriteMaterial"));
			sr->SetSprite(Resources::Find<graphics::Texture>(L"TITLE"));

			Model* model = new Model();
			if (model->LoadModel(L"..\\Resources\\character.fbx"))
			{
				mPlayer->AddComponent<ModelRenderer>();
				ModelRenderer* modelRenderer = mPlayer->GetComponent<ModelRenderer>();
				modelRenderer->SetMesh(model->GetMeshes());
			}
			else
			{
				delete model;
				model = nullptr;
			}
			
		}
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

			mPlayer = object::Instantiate<Player>(enums::eLayerType::BackGround, Vector3(0, 0, 0));


			SpriteRenderer* sr = mPlayer->AddComponent<SpriteRenderer>();
			sr->SetMaterial(Resources::Find<Material>(L"SpriteMaterial"));
			sr->SetSprite(Resources::Find<graphics::Texture>(L"TITLE"));

			Model* model = new Model();
			if (model->LoadModel(L"..\\Resources\\characterBase.fbx"))
			{
				mPlayer->AddComponent<ModelRenderer>();
				ModelRenderer* modelRenderer = mPlayer->GetComponent<ModelRenderer>();
				modelRenderer->SetMesh(model->GetMeshes());

				if(model->GetTextures().size() > 0)
					modelRenderer->SetTextures(model->GetTextures());
				else 
					modelRenderer->SetTexture( Resources::Find<graphics::Texture>(L"HUMAN"));

				
				modelRenderer->SetMaterial(Resources::Find<Material>(L"ModelMaterial"));
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
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
		mPlayer = object::Instantiate<Player>(enums::eLayerType::Player);

		SpriteRenderer* sr = mPlayer->AddComponent<SpriteRenderer>();
		sr->SetMaterial(Resources::Find<Material>(L"SpriteMaterial"));
		sr->SetSprite(Resources::Find<graphics::Texture>(L"TITLE"));



		UIManager::Push(enums::eUIType::Button);
		
		Scene::OnEnter();

	}

	void TitleScene::OnExit()
	{
		UIManager::Pop(enums::eUIType::Button);

		Scene::OnExit();

	}


}
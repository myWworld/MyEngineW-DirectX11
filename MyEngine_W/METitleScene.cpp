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
#include "MEUIManager.h"

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
		Vector2 resolution = Vector2(application.GetWidth(), application.GetHeight());

		GameObject* camera = object::Instantiate<GameObject>(enums::eLayerType::None, resolution / 2.0f);
		mCameraComp = camera->AddComponent<Camera>();
		renderer::mainCamera = mCameraComp;

		titleBg = object::Instantiate<GameObject>(enums::eLayerType::BackGround);

		SpriteRenderer* sr = titleBg->AddComponent<SpriteRenderer>();

		Transform* tr = titleBg->GetComponent<Transform>();
		tr->SetScale(Vector2(0.2f, 0.15f));


		

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

	void ME::TitleScene::Render(HDC mHdc)
	{
 		Scene::Render(mHdc);
	
	}

	void TitleScene::OnEnter()
	{
		MakeBG();

		renderer::mainCamera = mCameraComp;

		UIManager::Push(enums::eUIType::Button);
		
		Scene::OnEnter();

	}

	void TitleScene::OnExit()
	{
		UIManager::Pop(enums::eUIType::Button);

		Scene::OnExit();

	}

	void TitleScene::MakeBG()
	{
		graphics::Texture* titleTexture = Resources::Find<graphics::Texture>(L"TITLE");
		titleBg->GetComponent<SpriteRenderer>()->SetTexture(titleTexture);

		Transform* tr = titleBg->GetComponent<Transform>();
		tr->SetPosition(Vector2(190, 30));
	}

}
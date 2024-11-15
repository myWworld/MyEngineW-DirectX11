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
	GameObject* TitleScene::redTriangle = nullptr;

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

		redTriangle = object::Instantiate<GameObject>(enums::eLayerType::BackGround,Vector2(243, 127));
		SpriteRenderer* redTriangleSr = redTriangle->AddComponent<SpriteRenderer>();
		graphics::Texture* redTriangleTex = Resources::Find<graphics::Texture>(L"REDTRIANGLE");
		
		redTriangle->GetComponent<Transform>()->SetScale(Vector2(0.3, 0.3));
		redTriangleSr->SetTexture(redTriangleTex);


		redTriangle->SetNoRender(false);

		titleBg = object::Instantiate<GameObject>(enums::eLayerType::BackGround);

		SpriteRenderer* sr = titleBg->AddComponent<SpriteRenderer>();

		Transform* tr = titleBg->GetComponent<Transform>();
		tr->SetScale(Vector2(0.2f, 0.15f));


		graphics::Texture* titleTexture = Resources::Find<graphics::Texture>(L"TITLE");
		sr->SetTexture(titleTexture);

		Scene::Initialize();


	}

	void ME::TitleScene::Update()
	{
		Scene::Update();
	}

	void ME::TitleScene::LateUpdate()
	{
		Scene::LateUpdate();

		if (Input::GetKeyDown(eKeyCode::Enter))
		{
			SceneManager::LoadScene(L"Stage1");
		}

	}

	void ME::TitleScene::Render(HDC mHdc)
	{
 		Scene::Render(mHdc);
	
	}

	void TitleScene::OnEnter()
	{
		Transform* tr = titleBg->GetComponent<Transform>();
		tr->SetPosition(Vector2(190, 30));

		renderer::mainCamera = mCameraComp;

		UIManager::Push(enums::eUIType::Button);
		
		Scene::OnEnter();

	}

	void TitleScene::OnExit()
	{
		UIManager::Pop(enums::eUIType::Button);

		Scene::OnExit();

	}

}
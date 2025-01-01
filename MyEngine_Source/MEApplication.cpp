#include "MEApplication.h"
#include "MEInput.h"
#include "METime.h"
#include "MESceneManager.h"
#include "MEResources.h"
#include "MECollisionManager.h"
#include "MEUIManager.h"

#include "MEFmod.h"
#include "MERenderer.h"



namespace ME 
{

	Application::Application()
		:mHwnd(nullptr)
		, mHdc(nullptr)
		, mWidth(0)
		, mHeight(0)
		,mBackBuffer(nullptr)
		,mBackHdc(nullptr)
		,mSpeed(0)
		,mbLoaded(false)
	{

	}
	
	Application::~Application()
	{
		CoUninitialize();
	}




	void Application::Initialize(HWND hwnd, UINT width, UINT height)
	{
		AdjustWindowRect(hwnd, width, height);
		InitializeEtc();

		HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (FAILED(hr)) {
			MessageBox(nullptr, L"COM 초기화 실패!", L"Error", MB_OK);
			return;
		}

		 mGraphicDevice = std::make_unique<graphics::GraphicDevice_DX11>();
		 
		 renderer::Initialize();
		 mGraphicDevice->Initialize();
		 Fmod::Initialize();
		 CollisionManager::Iniatialize();
		 UIManager::Initailize();
		 SceneManager::Initialize();
		 
	}

	void Application::AdjustWindowRect(HWND hwnd, UINT width, UINT height)
	{
		mHwnd = hwnd;
		mHdc = GetDC(hwnd);
		RECT rect = { 0, 0, (LONG)width, (LONG)height };
		::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
		mWidth = rect.right - rect.left;
		mHeight = rect.bottom - rect.top;
		SetWindowPos(hwnd, nullptr, 0, 0, mWidth, mHeight, 0);
		ShowWindow(hwnd, true);
	}

	void Application::InitializeEtc()
	{
		Input::Initialize();
		Time::Intialize();
	}

	void Application::Run()
	{
		if (mbLoaded == false)
		{
			mbLoaded = true;
		}

		Update();
		LateUpdate();
		Render();

		Destroy();
	}

	void Application::Update()
	{
		Input::Update();
		Time::Update();

   		CollisionManager::Update();
		UIManager::Update();
		SceneManager::Update();

		
	}

	void Application::LateUpdate()
	{	
		CollisionManager::LateUpdate();
		UIManager::LateUpdate();
		SceneManager::LateUpdate();
	}

	void Application::Render()
	{
	
		graphics::GetDevice()->ClearRenderTargetView();
		graphics::GetDevice()->ClearDepthStencilView();
		graphics::GetDevice()->BindViewPort();
		graphics::GetDevice()->BindDefaultRenderTarget();


		Time::Render();
		
		CollisionManager::Render();
		SceneManager::Render();
		UIManager::Render();

		graphics::GetDevice()->Present();

		
	
	}

	void Application::Destroy()
	{
		SceneManager::Destroy();
	}

	void Application::Release()
	{
		SceneManager::Release();
		UIManager::Release();
		Resources::Release();


		renderer::Release();



	}



}

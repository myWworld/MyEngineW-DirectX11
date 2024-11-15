#pragma once

#include "CommonInclude.h"
#include "MEGameObject.h"
#include "MEScenes.h"



namespace ME
{
	

	class Application
	{

	public:

		Application();
		~Application();

		void Initialize(HWND hwnd, UINT width, UINT height);
		void Run();
		void Update();
		void LateUpdate();
		void Render();
		void Destroy();

		void Release();


		HDC GetHdc() { return mHdc; }
		HWND GetHwnd() { return mHwnd; }
		UINT GetWidth() { return mWidth; }
		UINT GetHeight() { return mHeight; }

		
	private:
		
		void ClearRenderTarget();
		void CopyRenderTarget(HDC source, HDC dest);
		void adjustWindowRect(HWND hwnd, UINT width, UINT height);
		void createBuffer(UINT width, UINT height);
		void initializeEtc();

		HWND mHwnd;
		HDC mHdc;

		HDC mBackHdc;
		HBITMAP mBackBuffer;

		UINT mWidth;
		UINT mHeight;

		float mSpeed;

		

		std::vector <Scene*> mScenes;
	};

}


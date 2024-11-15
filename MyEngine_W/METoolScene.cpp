#include "METoolScene.h"
#include "MEObject.h"
#include "METile.h"
#include "METileMapRenderer.h"
#include "MEResources.h"
#include "MERenderer.h"

#include "MEInput.h"
#include "MEApplication.h"

#include "MECameraScript.h"

extern ME::Application application;

namespace ME
{
	ToolScene::ToolScene()
	{
	}
	ToolScene::~ToolScene()
	{
	}
	void ToolScene::Initialize()
	{

		Vector2 resolution = Vector2(application.GetWidth(), application.GetHeight());

		GameObject* camera = object::Instantiate<GameObject>(enums::eLayerType::None, resolution/2.0f);
		mCameraComp = camera->AddComponent<Camera>();
		camera->AddComponent<CameraScript>();

		renderer::mainCamera = mCameraComp;

		Scene::Initialize();


	}
	void ToolScene::Update()
	{
		Scene::Update();
	}
	void ToolScene::LateUpdate()
	{
		Scene::LateUpdate();

		if (Input::GetKeyDown(eKeyCode::LeftMous))
		{
			Vector2 pos = Input::GetMousePos();
			pos = renderer::mainCamera->CalculateTilePosition(pos);
			
			if (pos.x >= 0.0f && pos.y >= 0.0f)
			{
				int idxX = pos.x / TileMapRenderer::TileSize.x;
				int idxY = pos.y / TileMapRenderer::TileSize.y;

				Tile* tile = object::Instantiate<Tile>(enums::eLayerType::Tile);
				TileMapRenderer* tmr = tile->AddComponent<TileMapRenderer>();

		
				tmr->SetTexture(Resources::Find<graphics::Texture>(L"SPRINGFLOOR"));
				tmr->SetIndex(TileMapRenderer::SelectedIndex);

				tile->SetIndexPosition(idxX, idxY);

				mTiles.push_back(tile);
			}
		}


		if (Input::GetKeyDown(eKeyCode::S))
		{
			Save();
		}

		if (Input::GetKeyDown(eKeyCode::L))
		{
			Load();
		}

	}
	void ToolScene::Render(HDC mHdc)
	{
		Scene::Render(mHdc);

		for (size_t i = 0; i < 50; i++)
		{
			Vector2 pos = renderer::mainCamera->CalculatePosition(Vector2(TileMapRenderer::TileSize.x * i, 0.0f));

			MoveToEx(mHdc, pos.x, 0, NULL);
			LineTo(mHdc, pos.x, 1000);

		}

		for (size_t i = 0; i < 50; i++)
		{

			Vector2 pos = renderer::mainCamera->CalculatePosition(Vector2(0.0f, TileMapRenderer::TileSize.y * i));

			MoveToEx(mHdc, 0, pos.y, NULL);
			LineTo(mHdc,1000, pos.y);

		}
	}
	void ToolScene::Destroy()
	{
		Scene::Destroy();
	}
	void ToolScene::OnEnter()
	{

		renderer::mainCamera = mCameraComp;
		Scene::OnEnter();
	}
	void ToolScene::OnExit()
	{
		Scene::OnExit();
	}
	void ToolScene::Save()
	{
		// open a file name
		OPENFILENAME ofn = {};

		wchar_t szFilePath[256] = {};

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFilePath;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = L"Tile\0*.tile\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (false == GetSaveFileName(&ofn))
			return;

		FILE* pFile = nullptr;

		_wfopen_s(&pFile, szFilePath, L"wb");
		
		for (Tile * tile: mTiles)
		{
			
			Transform* tr = tile->GetComponent<Transform>();
			TileMapRenderer* tmr = tile->GetComponent<TileMapRenderer>();

			Vector2 sourceIndex = tmr->GetIndex();

			Vector2 position = tr->GetPosition();

			int x = (int)sourceIndex.x;
		
			fwrite(&x, sizeof(int), 1, pFile);

			int y = (int)sourceIndex.y;

			fwrite(&y, sizeof(int), 1, pFile);

			x = (int)position.x;
	
			fwrite(&x, sizeof(int), 1, pFile);

			y = (int)position.y;
			fwrite(&y, sizeof(int), 1, pFile);

		}

		fclose(pFile);

	}

	void ToolScene::Load()
	{
		// open a file name
		OPENFILENAME ofn = {};

		wchar_t szFilePath[256] = {};

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFilePath;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = L"Tile\0*.tile\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (false == GetSaveFileName(&ofn))
			return;

		FILE* pFile = nullptr;

		_wfopen_s(&pFile, szFilePath, L"rb");

		while (true)
		{
			int idxX = 0;
			int idxY = 0;

			int posX = 0;
			int posY = 0;


			if (fread(&idxX, sizeof(int), 1, pFile) == NULL)
				break;
			if (fread(&idxY, sizeof(int), 1, pFile) == NULL)
				break;
			if (fread(&posX, sizeof(int), 1, pFile) == NULL)
				break;
			if (fread(&posY, sizeof(int), 1, pFile) == NULL)
				break;

			Tile* tile = object::Instantiate<Tile>(enums::eLayerType::Tile,Vector2(posX,posY));
			TileMapRenderer* tmr = tile->AddComponent<TileMapRenderer>();

			tmr->SetTexture(Resources::Find<graphics::Texture>(L"SPRINGFLOOR"));
			tmr->SetIndex(Vector2(idxX, idxY));

			mTiles.push_back(tile);
		}

		fclose(pFile);
	}
}


LRESULT CALLBACK WndTileProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		POINT mousePos = { };
		GetCursorPos(&mousePos);
		ScreenToClient(hWnd, &mousePos);

		ME::math::Vector2 mousePosition;
		mousePosition.x = mousePos.x;
		mousePosition.y = mousePos.y;
		int idxX = mousePosition.x / ME::TileMapRenderer::OriginTileSize.x;
		int idxY = mousePosition.y / ME::TileMapRenderer::OriginTileSize.y;
		ME::TileMapRenderer::SelectedIndex = Vector2(idxX, idxY);

		
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
		//DC란 화면에 출력에 필요한 모든 정보를 가지는 데이터 구조체
		//GDI모듈에 의해서 관리된다.
		//어떤 폰트를 사용할 건가, 어떤 선의 굵기를 정해줄건가 어떤 색상으로 그려줄껀가
		//화면 출력에 필요한 모든 경우는 WINAPI에서는 DC를 통해서 작업을 진행할 수 있다,

		ME::graphics::Texture* texture = ME::Resources::Find<ME::graphics::Texture>(L"SPRINGFLOOR");

		TransparentBlt(hdc
			, 0, 0
			, texture->GetWidth()
			, texture->GetHeight()
			, texture->GedHdc()
			, 0, 0
			, texture->GetWidth()
			, texture->GetHeight()
			, RGB(255, 0, 255));

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
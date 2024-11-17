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
	void ToolScene::Render()
	{
		Scene::Render();

	
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
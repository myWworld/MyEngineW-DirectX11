#include "MEUIManager.h"
#include "MEUIHUD.h"
#include "MEUIButton.h"

namespace ME
{
	std::unordered_map<enums::eUIType, UIBase*> UIManager::mUIs = {};
	std::stack<UIBase*> UIManager::mUIBases = {};
	std::queue<enums::eUIType> UIManager::mRequestUIQueue = {};
	UIBase* UIManager::mActiveUI =nullptr;

	void UIManager::Initailize()
	{
		UIHUD* hud = new UIHUD();
		mUIs.insert({ enums::eUIType::HpBar,hud });

		UIButton* button = new UIButton();
		mUIs.insert({ enums::eUIType::Button, button });
	}

	void UIManager::Render(HDC hdc)
	{
		std::stack<UIBase*> uiBases = mUIBases;

		while (!uiBases.empty())
		{
			UIBase* uiBase = uiBases.top();

			if (uiBase)
			{
				uiBase->Render(hdc);
				uiBases.pop();
			}


		}
	}
	void UIManager::OnLoad(enums::eUIType type)
	{
		std::unordered_map<enums::eUIType, UIBase*>::iterator iter
		= mUIs.find(type);

		if (iter == mUIs.end())
		{
			OnFail();
			return;	
		}

		OnComplete(iter->second);
	}

	void UIManager::Update()
	{
		std::stack<UIBase*> uiBases = mUIBases;

		while (!uiBases.empty())
		{
			UIBase* uiBase = uiBases.top();

			if (uiBase)
			{
				uiBase->Update();
				uiBases.pop();
			}


		}

		if (mRequestUIQueue.size() > 0)
		{
			enums::eUIType requestUI = mRequestUIQueue.front();
			mRequestUIQueue.pop();
			OnLoad(requestUI);
		}

	}
	void UIManager::LateUpdate()
	{
		std::stack<UIBase*> uiBases = mUIBases;

		while (!uiBases.empty())
		{
			UIBase* uiBase = uiBases.top();

			if (uiBase)
			{
				uiBase->LateUpdate();
				uiBases.pop();
			}


		}
	}
	void UIManager::OnComplete(UIBase* addUI)
	{
		if (addUI == nullptr)
			return;

		addUI->Initialize();
		addUI->Active();
		addUI->Update();

		//만약에 현재 추가된 ui가 전체화면이라면 그것말고 모두 비활성화

		if (addUI->IsFullScreen())
		{
			std::stack<UIBase*> uiBases = mUIBases;

			while (!uiBases.empty())
			{
				UIBase* uiBase = uiBases.top();
				uiBases.pop();

				if (uiBase)
				{
					uiBase->InActive();
					
				}


			}

		}

		mUIBases.push(addUI);
		mActiveUI = nullptr;

	}
	void UIManager::OnFail()
	{
		mActiveUI = nullptr;
	}
	void UIManager::Release()
	{
		for (auto iter : mUIs)
		{
			delete iter.second;
			iter.second = nullptr;
		}
	}

	void UIManager::Push(enums::eUIType type)
	{
		mRequestUIQueue.push(type);
	}

	void UIManager::Pop(enums::eUIType type)
	{
		if (mUIBases.size() <= 0)
			return;

		std::stack<UIBase*> tempStack;

		UIBase* uibase = nullptr;

		while (mUIBases.size() > 0)
		{
			uibase = mUIBases.top();
			mUIBases.pop();

			if (uibase->GetType() != type)
			{
				tempStack.push(uibase);
				continue;
			}

			if (uibase->IsFullScreen())
			{
				std::stack<UIBase*> uiBases = mUIBases;

				while (!uiBases.empty())
				{
					UIBase* uiBase = uiBases.top();
					uiBases.pop();

					if (uiBase)
					{
						uiBase->Active();
						break;
					}
				}
			}

			uibase->UIClear();


		}

		while (tempStack.size() > 0)
		{
			uibase = tempStack.top();
			tempStack.pop();
			mUIBases.push(uibase);
		}
	}




}
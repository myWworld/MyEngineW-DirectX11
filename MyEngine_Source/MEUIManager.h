#pragma once
#include "CommonInclude.h"
#include "../MyEngine_Source/MEUIBase.h"

namespace ME
{
	class UIManager
	{
	public:
		static void Initailize();
		static void Render();
		static void OnLoad(enums::eUIType type);
		static void Update();
		static void LateUpdate();
		static void OnComplete(UIBase* addUI);
		static void OnFail();
		static void Release();

		static void Push(enums::eUIType type);
		static void Pop(enums::eUIType type);

	private:

		static std::unordered_map<enums::eUIType, UIBase*> mUIs;
		static std::stack<UIBase*> mUIBases;
		static std::queue<enums::eUIType> mRequestUIQueue;

		static UIBase* mActiveUI;

	};

}

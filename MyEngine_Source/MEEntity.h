 #pragma once
#include "CommonInclude.h"

using namespace ME::math;


namespace ME
{
	class Entity
	{
	public:
		Entity();
		virtual ~Entity();

		void SetName(const std::wstring& name) { mName = name; }
		std::wstring& GetName() { return mName; }


	private:
		std::wstring mName;
	};
}


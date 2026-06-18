#pragma once
#include <iostream>
#include <unordered_map>
#include <array>
#include "MEEnums.h"

namespace ME
{
	static class BoneMapManager
	{
	public :


		
	public:

		static std::array<std::unordered_map<std::string, std::string>, static_cast<size_t>(enums::eBoneProfile::End)> mProfileBoneMaps	;
	};
}
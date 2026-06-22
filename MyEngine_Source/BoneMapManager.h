#pragma once
#include <iostream>
#include <unordered_map>
#include <array>
#include "MEEnums.h"
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

namespace ME
{
	static class BoneMapManager
	{
	public :

		static void LoadJsonBoneMap(const std::string& jsonFilePath, enums::eBoneProfile profile)
		{
			std::ifstream file(jsonFilePath);

			if (file.is_open())
			{
				int a = 9;
			}

			json datas = json::parse(file);

			std::unordered_map<std::string, std::string> boneMap;

			for (const auto& [key, vlaue] : datas.items())
			{
				boneMap[key] = vlaue.get<std::string>();
			}

			mProfileBoneMaps[static_cast<size_t>(profile)] = boneMap;
		}

		static const std::unordered_map<std::string, std::string>* GetBoneMap(enums::eBoneProfile profile)
		{
			if (profile == enums::eBoneProfile::None || profile >= enums::eBoneProfile::End)
			{
				std::cerr << "Invalid bone profile requested." << std::endl;
				return nullptr;
			}

			return &mProfileBoneMaps[static_cast<size_t>(profile)];
		}

		
	public:

		static std::array<std::unordered_map<std::string, std::string>, static_cast<size_t>(enums::eBoneProfile::End)> mProfileBoneMaps	;
	};
}
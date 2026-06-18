#pragma once
#include "MEResource.h"
#include "json.hpp"
#include "StringUtility.h"
#include <memory>

using json = nlohmann::json;

namespace ME
{

	class Resources
	{
	public:


		
		template <typename T>
		static std::shared_ptr<T> Find(const std::wstring& key)
		{
			auto iter = mResources.find(key);
		
			if (iter == mResources.end())
				return nullptr;

			return std::dynamic_pointer_cast<T>(iter->second);
		}
	


		template <typename T>
		static std::shared_ptr<T> Load(const std::wstring& key, const std::wstring &path)
		{
			std::shared_ptr<T> resource = Resources::Find<T>(key);
			if (resource != nullptr)
				return resource;

			resource = std::make_shared<T>();

			if (FAILED(resource->Load(path)))
			{
				assert(false);
			}

			resource->SetName(key);
			resource->SetPath(path);
			mResources.emplace(key,resource );
			
			return resource;
		}

		static void Insert(const std::wstring& key, std::shared_ptr<Resource> resource)
		{
			if (key == L"")
				return;
			if (resource == nullptr)
				return;

			mResources.insert({ key, resource });
		}

		static void Release()
		{
			mResources.clear();
		}

		template<typename T>
		static void LoadFromJSON(const json& data, const std::string& category)
		{
			if (!data.contains(category)) return;

			for (const auto& item : data[category])
			{

				if (!item.contains("key") || !item.contains("path"))
				{
					assert(false && "Invalid JSON format for resource loading.");
					continue;
				}

				Load<T>(
					StringUtility::s2ws(item["key"].get<std::string>()),
					StringUtility::s2ws(item["path"].get<std::string>())
				);
			}
		}
		
	private:

		static std::unordered_map<std::wstring, std::shared_ptr<Resource>> mResources;
	};
}

#pragma once
#include <string>

namespace ME
{
	static class StringUtility
	{
	public:
		static std::wstring s2ws(const std::string& str)
		{
			return std::wstring(str.begin(), str.end());
		}

		static std::string ws2s(const std::wstring& wstr)
		{
			return std::string(wstr.begin(), wstr.end());
		}

		static std::string extractBoneName(const std::string& name)
		{
			std::string newName = name;
			size_t pos = newName.find_last_of("/|:");
			if (pos != std::string::npos)
			{
				newName = newName.substr(pos + 1);
			}
			size_t assimpFbxPos = newName.find("$AssimpFbx$");
			if (assimpFbxPos != std::string::npos)
			{
				newName = newName.substr(0, assimpFbxPos - 1);
			}
			return newName;
		}
	};
}
#pragma once
#include <vector>
#include <string>

namespace StringUtility
{
	void split(const std::string& source, const char split, std::vector<std::string>& result);
	std::wstring ToWString(const std::string& str);
}
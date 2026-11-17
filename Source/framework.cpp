#include "framework.h"

std::wstring ToWideString(const std::string& str)
{
	if (str.empty())
		return {};

	int size = MultiByteToWideChar(
		CP_UTF8, 0, str.c_str(), -1, nullptr, 0
	);

	std::wstring result(size, L'\0');

	MultiByteToWideChar(
		CP_UTF8, 0, str.c_str(), -1, result.data(), size
	);

	result.pop_back(); // Remove null terminator

	return result;
}

std::filesystem::path GetExecutableDirectory()
{
	wchar_t buffer[MAX_PATH];

	GetModuleFileNameW(nullptr, buffer, MAX_PATH);

	return std::filesystem::path(buffer).parent_path();
}

std::filesystem::path GetResourcePath(const std::filesystem::path& relativePath)
{
	return GetExecutableDirectory() / relativePath;
}

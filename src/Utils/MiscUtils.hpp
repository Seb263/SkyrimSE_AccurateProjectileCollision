#pragma once

#include "DataHandler.hpp"

class MiscUtils
{
	public:

	static REL::Version GetPluginVersion(const char* a_moduleName)
	{
		const auto handle = GetModuleHandleA(a_moduleName);
		if (!handle) return REL::Version{};

		char path[MAX_PATH]{};
		if (!GetModuleFileNameA(handle, path, MAX_PATH)) return REL::Version{};

		DWORD dummy = 0;
		const DWORD size = GetFileVersionInfoSizeA(path, &dummy);
		if (size == 0) return REL::Version{};

		std::vector<std::byte> data(size);
		if (!GetFileVersionInfoA(path, 0, size, data.data())) return REL::Version{};

		VS_FIXEDFILEINFO* fileInfo = nullptr;
		UINT fileInfoLen = 0;
		if (!VerQueryValueA(data.data(), "\\", reinterpret_cast<LPVOID*>(&fileInfo), &fileInfoLen)) return REL::Version{};
		if (!fileInfo) return REL::Version{};

		return REL::Version{
			HIWORD(fileInfo->dwFileVersionMS),
			LOWORD(fileInfo->dwFileVersionMS),
			HIWORD(fileInfo->dwFileVersionLS),
			LOWORD(fileInfo->dwFileVersionLS)
		};
	}
};

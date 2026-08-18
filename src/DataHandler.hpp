#pragma once

#include "API/Precision-API.h"

namespace ModData
{
	constexpr std::string_view MOD_NAME = "Accurate Projectile Collision";

	inline auto lastLoadPoint = std::chrono::steady_clock::now();

	struct PluginForm
	{
		std::string_view name;
		void** formPtr;
		uint32_t formID;
		std::string_view pluginName;
		bool optional = false;
	};

	struct DefaultForm
	{
		void** formPtr;
		std::string formStr;
	};

	// Properties storing game form references
	static inline const std::vector<PluginForm> pluginForms = {};

	inline RE::TESDataHandler* TESdataHandler;

	inline PRECISION_API::IVPrecision4* Precision_API_Interface = nullptr;
}

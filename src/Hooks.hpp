#pragma once

#include "DataHandler.hpp"
#include "SettingsIni.hpp"

#include "API/Precision-API.h"

#include "Core/Main.hpp"

namespace Events
{
	using namespace ModData;
	using namespace ModCore;

	class Hooks
	{
	public:
		static void InitializePrecision()
		{
			using namespace ModData;
			using namespace ModCore;

			if (!Precision_API_Interface) return;

			Precision_API_Interface->AddCollisionFilterComparisonCallback(SKSE::GetPluginHandle(), Main::CompareFilterInfoCallback);
			Precision_API_Interface->AddPrecisionLayerSetupCallback(SKSE::GetPluginHandle(), Main::PrecisionLayerSetupCallback);
		}
	};
};

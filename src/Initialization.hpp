#pragma once

#include "DataHandler.hpp"
#include "Hooks.hpp"
#include "SettingsIni.hpp"

#include "API/Precision-API.h"

#include "Utils/MiscUtils.hpp"

namespace ModData
{
	class DataHandler
	{
	public:
		bool preLoaded = false;
		bool postLoaded = false;
		bool postLoadedAlternate = false;

		static DataHandler* GetSingleton()
		{
			static DataHandler singleton;
			return &singleton;
		}

		void PreLoadData()
		{
			if (preLoaded) return;
			preLoaded = true;

			TESdataHandler = RE::TESDataHandler::GetSingleton();

			LoadPluginsForms();
		}

		void PostLoadData()
		{
			if (postLoaded) return;
			postLoaded = true;

			if (REL::Module::IsVR()) {
				logger::error("This mod is not compatible with the VR edition. Aborting.");
				return;
			}

			if (!LoadPrecisionApi()) return;

			Events::Hooks::InitializePrecision();
		}

	private:
		static inline void LoadPluginsForms()
		{
			logger::info("Loading Plugins Froms Data...");

			for (const auto& formInfo : pluginForms) {
				*formInfo.formPtr = TESdataHandler->LookupForm(formInfo.formID, formInfo.pluginName.data());
				if (!*formInfo.formPtr && !formInfo.optional) {
					REPORT_AND_FAIL("ERROR: Form \"{}\" not found in \"{}\".", formInfo.pluginName, formInfo.name, formInfo.pluginName);
				}
			}

			logger::info("Loading Plugins Froms Data: DONE");
		}

		static inline bool LoadPrecisionApi()
		{
			constexpr REL::Version kRequiredVersion{ 2, 0, 4, 0 };
			const auto dllVersion = MiscUtils::GetPluginVersion("Precision.dll");
			const bool versionOk = dllVersion != REL::Version{} && dllVersion >= kRequiredVersion;

			auto* apiInterface = versionOk ? static_cast<PRECISION_API::IVPrecision4*>(PRECISION_API::RequestPluginAPI(PRECISION_API::InterfaceVersion::V4)) : nullptr;

			if (!apiInterface) {
				logger::error("Precision API not found or version insufficient.");

				const std::string title = fmt::format("{}: Missing Requirement", MOD_NAME);
				const std::string msg_box = fmt::format(
					"Precision version {} or higher is required to run {}.\n\n"
					"Would you like to close the game and open the download page?",
					kRequiredVersion.string("."), MOD_NAME);

				if (REX::W32::MessageBoxA(nullptr, msg_box.c_str(), title.c_str(), MB_ICONWARNING | MB_YESNO) == IDYES) {
					::ShellExecuteA(nullptr, "open", "https://www.seb263.fr/short-url/precision", nullptr, nullptr, SW_SHOWNORMAL);
					REX::W32::TerminateProcess(REX::W32::GetCurrentProcess(), EXIT_FAILURE);
				}
				return false;
			}

			Precision_API_Interface = apiInterface;
			logger::info("Precision API v4 registered successfully.");

			return true;
		}
	};
}

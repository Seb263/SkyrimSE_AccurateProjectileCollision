#pragma once

#include "DataHandler.hpp"
#include "SettingsIni.hpp"

#include "API/Precision-API.h"

namespace ModCore
{
	class Main
	{
	public:
		static PRECISION_API::CollisionFilterComparisonResult CompareFilterInfoCallback(
			[[maybe_unused]] RE::bhkCollisionFilter* a_collisionFilter, uint32_t a_filterInfoA, uint32_t a_filterInfoB)
		{
			using namespace ModData;

			const auto layerA = static_cast<RE::COL_LAYER>(a_filterInfoA & 0x7F);
			const auto layerB = static_cast<RE::COL_LAYER>(a_filterInfoB & 0x7F);

			const bool involvesCharController = (layerA == RE::COL_LAYER::kCharController || layerB == RE::COL_LAYER::kCharController);
			if (!involvesCharController) return PRECISION_API::CollisionFilterComparisonResult::Continue;

			const RE::COL_LAYER otherLayer = (layerA == RE::COL_LAYER::kCharController) ? layerB : layerA;

			const bool isProjectile = (otherLayer == RE::COL_LAYER::kProjectile);
			const bool isSpellLike = (otherLayer == RE::COL_LAYER::kSpell || otherLayer == RE::COL_LAYER::kConeProjectile);

			if (!isSpellLike && !isProjectile) return PRECISION_API::CollisionFilterComparisonResult::Continue;

			if (isProjectile && !SettingsIni::bMain_EnableArrow) {
				return PRECISION_API::CollisionFilterComparisonResult::Continue;
			}
			if (isSpellLike && !SettingsIni::bMain_EnableSpell) {
				return PRECISION_API::CollisionFilterComparisonResult::Continue;
			}

			const uint32_t charControllerFilter = (layerA == RE::COL_LAYER::kCharController) ? a_filterInfoA : a_filterInfoB;
			const uint16_t charControllerGroup = static_cast<uint16_t>(charControllerFilter >> 16);

			if (Precision_API_Interface->IsActorActiveCollisionGroup(charControllerGroup) &&
			    !Precision_API_Interface->IsCharacterControllerHittableCollisionGroup(charControllerGroup)) {
				return PRECISION_API::CollisionFilterComparisonResult::Ignore;
			}

			return PRECISION_API::CollisionFilterComparisonResult::Continue;
		}

		static PRECISION_API::PrecisionLayerSetupCallbackReturn PrecisionLayerSetupCallback()
		{
			using namespace ModData;

			PRECISION_API::PrecisionLayerSetupCallbackReturn result{};
			result.precisionLayerType = PRECISION_API::PrecisionLayerType::Body;

			if (SettingsIni::bMain_EnableArrow) {
				result.layersToAdd |= (static_cast<uint64_t>(1) << static_cast<uint64_t>(RE::COL_LAYER::kConeProjectile));
				result.layersToAdd |= (static_cast<uint64_t>(1) << static_cast<uint64_t>(RE::COL_LAYER::kProjectile));
			}

			if (SettingsIni::bMain_EnableSpell) {
				result.layersToAdd |= (static_cast<uint64_t>(1) << static_cast<uint64_t>(RE::COL_LAYER::kSpell));
			}

			return result;
		}
	};
}

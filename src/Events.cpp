#include "Events.h"

namespace Events
{
	RE::BSEventNotifyControl ModEventSink::ProcessEvent(const RE::TESLoadGameEvent* event, RE::BSTEventSource<RE::TESLoadGameEvent>*)
	{
		using namespace ModData;

		ModData::lastLoadPoint = std::chrono::steady_clock::now();

		return continueEvent;
	}
}

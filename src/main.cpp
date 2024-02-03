#include "luare/luaRE.h"

#include "bindings/RE/nipoint.h"
#include "bindings/RE/actor.h"
#include "bindings/RE/form.h"

#include <sol/sol.hpp>


class TESObjectLoadedEventHandler :
	public dku::model::Singleton<TESObjectLoadedEventHandler>,
	public RE::BSTEventSink<RE::TESObjectLoadedEvent>
{
public:
	using EventResult = RE::BSEventNotifyControl;

	virtual EventResult ProcessEvent(const RE::TESObjectLoadedEvent* a_event, RE::BSTEventSource<RE::TESObjectLoadedEvent>* a_eventSource) override
	{
		if (!a_event || !a_eventSource) {
			return EventResult::kContinue;
		}

		if (a_event->loaded) {
			luare::events::dispatch("TESObjectLoadedEvent", a_event->formID);
		}

		return EventResult::kContinue;
	}
};


namespace
{
	static int add(int a, int b) { return a + b; }

	void InitLuaRE()
	{
		// init luare server
		luare::prepare_lua();
		luare::register_native_handler();

		// enable console
		std::jthread repl{
			[] { luare::console::start_repl(); }
		};
		repl.detach(); 

		// userclass exports
		LUARE_CLASS(bindings::nipoint, 
			LUARE_CTOR(bindings::nipoint, bindings::actor(float, float, float)),
			LUARE_MEMBER(bindings::nipoint, x),
			LUARE_MEMBER(bindings::nipoint, y),
			LUARE_MEMBER(bindings::nipoint, z),
			// parent class method
			LUARE_MEMBER(bindings::nipoint, Cross));

		LUARE_CLASS(bindings::form, 
			// 1 ctor
			LUARE_CTOR(bindings::form, bindings::form(RE::FormID)),
			// member field
			LUARE_MEMBER(bindings::form, impl));

		LUARE_CLASS(bindings::actor,
			// 2 ctors
			LUARE_CTOR(bindings::actor, bindings::actor(RE::FormID), bindings::actor(bindings::form)),
			// member func
			LUARE_MEMBER(bindings::actor, getpos));

		// init lua client scripts
		luare::load_base_scripts();

		luare::luare_loaded = true;
	}
}


void MessageCallback(SKSE::MessagingInterface::Message* a_msg)
{
	if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {
		auto* source = RE::ScriptEventSourceHolder::GetSingleton();
		source->AddEventSink(TESObjectLoadedEventHandler::GetSingleton());
	}
}


DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
#ifndef NDEBUG
	//while (!IsDebuggerPresent()) { Sleep(100); }
#endif

	DKUtil::Logger::Init(Plugin::NAME, REL::Module::get().version().string());

	//REL::Module::reset();
	SKSE::Init(a_skse);
	
	INFO("{} v{} loaded", Plugin::NAME, Plugin::Version);

	// do stuff
	InitLuaRE();
	SKSE::GetMessagingInterface()->RegisterListener(MessageCallback);

	return true;
}


#if defined(LUARE_RUNTIME_BUILD)
int main(int a_argc, const char* a_argv[])
{
	api::prepare_lua(api::this_lua);
	api::console::start_repl(api::this_lua);
	api::load_base_scripts();
	api::luare_loaded = true;
	
	return 1;
}
#endif
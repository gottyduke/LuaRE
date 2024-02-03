#pragma once

#include "native.h"
#include "luare/luaRE.h"

#include <sol/sol.hpp>

namespace luare
{
	namespace native
	{
		void run(std::string a_cmd) noexcept
		{
			const auto script = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>()->Create();
			script->SetCommand(a_cmd);
			script->CompileAndRun(nullptr);
		}

		void log(std::string a_log) noexcept
		{
			INFO("[luare] {}", a_log);
		}
	} // namespace native

	void register_native_handler() noexcept
	{
		auto skyrim = this_lua.create_named_table("skyrim");
		skyrim.set_function("run", native::run);

		auto native = this_lua.create_named_table("native");
		native.set_function("log", native::log);
	}
}  // namespace api
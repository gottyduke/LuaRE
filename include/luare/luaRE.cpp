#include "luaRE.h"

#include "exports.h"
#include <sol/sol.hpp>

namespace luare
{
	void prepare_lua() noexcept
	{
		this_lua.open_libraries();
		append_library_lookup(LUARE_PATH);
	}

	bool assert_stack_space(int a_size) noexcept
	{
		return sol::stack::get<sol::object>(this_lua, a_size).valid();
	}

	void dump_stack() noexcept
	{
		int n = this_lua.stack_top();
		if (n > 0) {
			if (assert_stack_space(LUA_MINSTACK)) {
				ERROR("too many values on stack to print");
			}
			auto p = this_lua["print"];
			p.push();
			lua_insert(this_lua, 1);
			lua_pcall(this_lua, n, 0, 0);
		}
	}
} // namespace api

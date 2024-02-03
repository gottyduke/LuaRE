#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#define LUARE_MEMBER(C, P) luare::strip_export_name(#P), &C::P
#define LUARE_CTOR_EX(C, ...) sol::constructors<__VA_ARGS__>()
#define LUARE_CTOR(C, ...) luare::strip_export_name(#C), LUARE_CTOR_EX(C, __VA_ARGS__)
#define LUARE_CLASS(C, ...) luare::this_lua.new_usertype<C>(__VA_ARGS__)
#define LUARE_FUNCTION(F) luare::this_lua.set_function(luare::strip_export_name(#F), F)

namespace luare
{
	inline static constexpr auto LUARE_PATH = "Data\\SKSE\\Plugins\\LuaRE";

	extern sol::state this_lua;
	extern bool luare_loaded;

	std::string strip_export_name(std::string a_export) noexcept;
}  // namespace api
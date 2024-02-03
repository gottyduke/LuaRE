#include "fs.h"

#include "luare/exports.h"
#include "luare/console.h"

#include <sol/sol.hpp>
#include "DKUtil/Config.hpp"

namespace luare
{
	namespace detail
	{
		std::vector<std::string> get_directories(const std::filesystem::path& a_path)
		{
			std::vector<std::string> dir;
			for (auto& p : std::filesystem::recursive_directory_iterator(a_path))
				if (p.is_directory())
					dir.push_back(p.path().string());
			return dir;
		}

		sol::protected_function_result on_error_report_script(lua_State* L, sol::protected_function_result a_pfr)
		{
			auto err = sol::stack::get<std::string_view>(this_lua, a_pfr.stack_index());
			COUT("{}", err);

			return a_pfr;
		}
	} // namespace detail

	void load_base_scripts() noexcept
	{
		for (auto& f : dku::Config::GetAllFiles<true>(LUARE_PATH, ".lua")) {
			INFO("loading script {}", f);
			this_lua.script_file(f, &detail::on_error_report_script);
		}
	}

	std::string append_library_lookup(std::string_view a_appendix, bool a_addSubdir) noexcept
	{
		auto fp = std::filesystem::absolute(a_appendix);
		auto lookup = fmt::format("{}{}?.lua", fp.string(), fp.string().ends_with("\\") ? "" : "\\");

		if (a_addSubdir) {
			for (auto& sd : detail::get_directories(fp)) {
				lookup += fmt::format(";{}{}?.lua", sd, sd.ends_with("\\") ? "" : "\\");
			}
		}

		const std::string packages = this_lua["package"]["path"];
		this_lua["package"]["path"] = packages + (!packages.empty() ? ";" : "") + lookup;

		return this_lua["package"]["path"];
	}
}  // namespace api

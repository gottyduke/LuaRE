#include "exports.h"

#include <sol/sol.hpp>

namespace luare
{
	sol::state this_lua{};
	bool luare_loaded{ false };

	std::string strip_export_name(std::string a_export) noexcept
	{
		auto stripped = dku::string::split(a_export, "::", "&");
		return stripped.back();
	}
}  // namespace api

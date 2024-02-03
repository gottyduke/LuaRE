#pragma once

#include "exports.h"

namespace luare::events
{
	template <typename... Args>
	inline static void dispatch(std::string_view a_event, Args&&... a_args) noexcept
	{
		static auto dispatcher = this_lua["events"]["dispatch"];
		dispatcher(a_event, std::forward<Args>(a_args)...);
	}
} // namespace api
#pragma once

#include "exports.h"

#define COUT(...)                            \
	{                                        \
		auto fmt = fmt::format(__VA_ARGS__); \
		std::cout << fmt << std::endl;       \
		INFO("[luare] {}", fmt);             \
	}

namespace luare::console
{
	// start a lua repl, auto alloc console if needed
	// use std::jthread for non-blocking console
	void start_repl() noexcept;
	// stop the current repl
	void stop_repl() noexcept;
}  // namespace api::console

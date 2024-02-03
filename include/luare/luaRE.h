#pragma once

#include "exports.h"
#include "console.h"
#include "fs.h"
#include "events.h"
#include "native.h"

namespace luare
{
	// check if enough stack frame available
	bool assert_stack_space(int a_size) noexcept;
	// insert stack dumper, this clears stack
	void dump_stack() noexcept;
	// used internally to prepare lua server
	void prepare_lua() noexcept;
} // namespace api

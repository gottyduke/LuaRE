#pragma once

namespace luare
{
	// load bundled scripts
	void load_base_scripts() noexcept;
	// append a lua module lookup path
	std::string append_library_lookup(std::string_view a_appendix, bool a_addSubdir = false) noexcept;
}  // namespace api
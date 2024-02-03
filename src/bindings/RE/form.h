#pragma once

namespace bindings
{
	class form
	{
	public:
		// lookup by id
		form(RE::FormID a_id) noexcept;

		RE::TESForm* impl;
	};
}  // namespace bindings
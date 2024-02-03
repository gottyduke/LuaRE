#pragma once

#include "form.h"
#include "nipoint.h"

namespace bindings
{
	class actor : public form
	{
	public:
		actor(RE::FormID a_id) noexcept :
			form(a_id), pos(0, 0, 0){};

		actor(form a_form) noexcept :
			form(a_form), pos(0, 0, 0){};

		nipoint getpos() noexcept;

	private:
		nipoint pos;
	};
}  // namespace bindings
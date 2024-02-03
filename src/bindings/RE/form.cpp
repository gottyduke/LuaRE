#include "form.h"

#include "luare/exports.h"
#include "luare/console.h"

namespace bindings
{
	form::form(RE::FormID a_id) noexcept
	{
		impl = RE::TESForm::LookupByID(a_id);
	}
}  // namespace bindings
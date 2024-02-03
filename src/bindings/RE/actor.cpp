#include "actor.h"

#include "luare/exports.h"
#include "luare/console.h"

namespace bindings
{
	nipoint actor::getpos() noexcept
	{
		auto* re_actor = impl->As<RE::Character>();
		pos = re_actor->GetPosition();
		return pos;
	}
}  // namespace bindings
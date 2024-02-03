#pragma once

#include "luare/exports.h"

namespace bindings
{
	class nipoint : public RE::NiPoint3
	{
	public:
		constexpr nipoint(float a_x, float a_y, float a_z) noexcept :
			NiPoint3(a_x, a_y, a_z)
		{};

		// not exported
		constexpr nipoint(RE::NiPoint3 a_rhs) noexcept :
			NiPoint3(a_rhs){};
	};
}  //namespace bindings
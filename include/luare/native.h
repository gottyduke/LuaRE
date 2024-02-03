#pragma once

namespace luare
{
	void register_native_handler() noexcept;

	namespace native
	{
		void run(std::string a_cmd) noexcept;
	} // namespace native
} // namespace api
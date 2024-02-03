#include "console.h"

#include <sol/sol.hpp>
#include <io.h>
#include <fcntl.h>

namespace luare::console
{
	namespace detail
	{
		void BindCrtHandlesToStdHandles(bool bindStdIn, bool bindStdOut, bool bindStdErr)
		{
			if (bindStdIn) {
				FILE* dummyFile;
				freopen_s(&dummyFile, "nul", "r", stdin);
			}
			if (bindStdOut) {
				FILE* dummyFile;
				freopen_s(&dummyFile, "nul", "w", stdout);
			}
			if (bindStdErr) {
				FILE* dummyFile;
				freopen_s(&dummyFile, "nul", "w", stderr);
			}

			if (bindStdIn) {
				HANDLE stdHandle = GetStdHandle(STD_INPUT_HANDLE);
				if (stdHandle != INVALID_HANDLE_VALUE) {
					int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
					if (fileDescriptor != -1) {
						FILE* file = _fdopen(fileDescriptor, "r");
						if (file != NULL) {
							int dup2Result = _dup2(_fileno(file), _fileno(stdin));
							if (dup2Result == 0) {
								setvbuf(stdin, NULL, _IONBF, 0);
							}
						}
					}
				}
			}

			if (bindStdOut) {
				HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
				if (stdHandle != INVALID_HANDLE_VALUE) {
					int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
					if (fileDescriptor != -1) {
						FILE* file = _fdopen(fileDescriptor, "w");
						if (file != NULL) {
							int dup2Result = _dup2(_fileno(file), _fileno(stdout));
							if (dup2Result == 0) {
								setvbuf(stdout, NULL, _IONBF, 1000);
							}
						}
					}
				}
			}

			if (bindStdErr) {
				HANDLE stdHandle = GetStdHandle(STD_ERROR_HANDLE);
				if (stdHandle != INVALID_HANDLE_VALUE) {
					int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
					if (fileDescriptor != -1) {
						FILE* file = _fdopen(fileDescriptor, "w");
						if (file != NULL) {
							int dup2Result = _dup2(_fileno(file), _fileno(stderr));
							if (dup2Result == 0) {
								setvbuf(stderr, NULL, _IONBF, 0);
							}
						}
					}
				}
			}

			if (bindStdIn) {
				std::wcin.clear();
				std::cin.clear();
			}
			if (bindStdOut) {
				std::wcout.clear();
				std::cout.clear();
			}
			if (bindStdErr) {
				std::wcerr.clear();
				std::cerr.clear();
			}
		}

		void try_alloc_console() noexcept
		{
			AllocConsole();
			BindCrtHandlesToStdHandles(true, true, true);
			SetConsoleOutputCP(CP_UTF8);
			SetConsoleTitleA("LuaRE");
		}

		sol::load_status try_compile(std::string_view a_input) noexcept
		{
			auto chunk = this_lua.load(a_input);
			if (chunk.valid()) {
				chunk();
			}

			sol::stack::remove(this_lua, chunk.stack_index(), 1);
			return chunk.status();
		}

		sol::protected_function_result on_error_check_syntax_eof(lua_State* L, sol::protected_function_result a_pfr) noexcept
		{
			auto err = sol::stack::get<std::string_view>(this_lua, a_pfr.stack_index());
			sol::stack::push<bool>(this_lua, err.ends_with("<eof>"));

			if (!err.ends_with("<eof>")) {
				std::cerr << err << std::endl;
			}

			return a_pfr;
		}
	} // namespace detail
	

	void start_repl() noexcept
	{
		detail::try_alloc_console();

		std::string buffer{};
		std::vector<std::string> history{};
		bool multiline{ false };

		while (std::cout << (multiline ? "... " : ">>> ") && std::getline(std::cin, buffer)) {
			if (buffer.empty()) {
				continue;
			}

			if (buffer.size() >= 512) {
				std::cout << "max length exceeds lua input 512";
				buffer.clear();
				multiline = false;
				continue;
			}

			if (buffer.ends_with('\n')) {
				buffer.pop_back();
			}

			// multiline
			if (multiline) {
				buffer = fmt::format("{}\n{}", history.back(), buffer);
				history.pop_back();
			}
			history.push_back(buffer);

			// single line
			auto pfr = detail::try_compile(buffer);
			if (pfr == sol::load_status::syntax) {
				// init multiline
				this_lua.script(buffer, &detail::on_error_check_syntax_eof);
				multiline = sol::stack::pop<bool>(this_lua);
				continue;
			}

			std::cout << dku::print_enum(pfr) << std::endl;

			buffer.clear();
			multiline = false;
		}
	}

	void stop_repl() noexcept
	{
		FreeConsole();
	}
} // namespace api

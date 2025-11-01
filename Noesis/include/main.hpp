#include <iostream>
#include <ws2tcpip.h>
#include <windows.h>
#include <winbase.h>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <format>
#include <string>
#include <chrono>
#include <ctime>


#pragma comment(lib, "Ws2_32.lib")


class Logger {

public:
	enum class Level {
		OK,
		WARN,
		ERR,
		INPUT
	};
	template <typename... Args>
	static void log(Level lvl, std::string_view message, Args&&... args)
	{
		std::string prefix = getPrefix(lvl);
		std::string formatted = std::vformat(message, std::make_format_args<Args>(args)...));

		std::cout << prefix << formatted << std::endl;
	}
	template <typename... Args>
	static void ok(std::string_view msg, Args&&... args) {
		log(Level::OK, msg, std::forward<Args>(args)...);
	}
	template <typename... Args>
	static void warn(std::string_view msg, Args&&... args) {
		log(Level::WARN, msg, std::forward<Args>(args)...);
	}
	template <typename... Args>
	static void error(std::string_view msg, Args&&... args) {
		log(Level::ERR, msg, std::forward<Args>(args)...);
	}
	template <typename... Args>
	static void input(std::string_view msg, Args&&... args) {
		log(Level::INPUT, msg, std::forward<Args>(args)...);
	}
private:
	static std::string getPrefix(Level lvl)
	{
		switch (lvl) {
		case Level::OK: return "[+]";
		case Level::WARN: return "[!]";
		case Level::ERR: return "[-]";
		case Level::INPUT: return "[->]";
		default:	 return "[]";
		}
	}
};

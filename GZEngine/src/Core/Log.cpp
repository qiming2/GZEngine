#include "gzpch.h"
#include "Log.h"


namespace GZ {
	std::shared_ptr<spdlog::logger> Log::s_core_logger;
	std::shared_ptr<spdlog::logger> Log::s_app_logger;
	//g_logger = spdlog::stdout_color_mt("Global");;
	std::shared_ptr<spdlog::logger> g_logger;
	void Log::init()
	{
		g_logger = spdlog::stdout_color_mt("Global");
		g_logger->set_level(spdlog::level::trace);

		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_core_logger = spdlog::stdout_color_mt("Core");
		s_core_logger->set_level(spdlog::level::trace);

		s_app_logger = spdlog::stdout_color_mt("App");
		s_app_logger->set_level(spdlog::level::trace);
	}
}


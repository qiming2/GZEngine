#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace GZ {

	class Log {
	public:
		static void init();
		static std::shared_ptr<spdlog::logger> s_core_logger;
		static std::shared_ptr<spdlog::logger> s_app_logger;
	};
}

#ifdef GZ_PLATFORM_WINDOWS
#define gz_debugbreak __debugbreak()
#else
#define gz_debugbreak raise(SIGTRAP)
#endif

#define gz_trace(...)  GZ::Log::s_app_logger->trace(__VA_ARGS__)
#define gz_info(...)  GZ::Log::s_app_logger->info(__VA_ARGS__)
#define gz_warn(...)  GZ::Log::s_app_logger->warn(__VA_ARGS__)
#define gz_error(...)  do { \
						GZ::Log::s_app_logger->error(__VA_ARGS__);  \
						gz_debugbreak; \
					   } while(0)
#define gz_critical(...)  GZ::Log::s_app_logger->critical(__VA_ARGS__)

#define gz_core_trace(...)  GZ::Log::s_core_logger->trace(__VA_ARGS__)
#define gz_core_info(...)  GZ::Log::s_core_logger->info(__VA_ARGS__)
#define gz_core_warn(...)  GZ::Log::s_core_logger->warn(__VA_ARGS__)
#define gz_core_error(...)  do { \
						GZ::Log::s_core_logger->error(__VA_ARGS__);  \
						gz_debugbreak; \
					   } while(0)
#define gz_core_critical(...)  GZ::Log::s_core_logger->critical(__VA_ARGS__)
#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "LogCommonFormatter.h"

namespace GZ {

    //extern std::shared_ptr<spdlog::logger> g_logger;
	struct GZ_API Log {
	public:
		static void init();
		static std::shared_ptr<spdlog::logger> s_core_logger;
		static std::shared_ptr<spdlog::logger> s_app_logger;
	};
}

#ifdef GZ_PLATFORM_WINDOWS
#define gz_debugbreak __debugbreak()
#else
#include <signal.h>
#define gz_debugbreak raise(SIGTRAP)
#endif

#ifndef GZ_DSIT
#define gz_assert(expr, ...) do { \
                                if (!(expr)) { \
                                    GZ::Log::s_app_logger->error(__VA_ARGS__);  \
                                    gz_debugbreak; \
                                }\
                             } while(0)
#define gz_trace(...) GZ::Log::s_app_logger->trace(__VA_ARGS__)
#define gz_info(...) GZ::Log::s_app_logger->info(__VA_ARGS__)
#define gz_warn(...) GZ::Log::s_app_logger->warn(__VA_ARGS__)
#define gz_error(...) do { \
						GZ::Log::s_app_logger->error(__VA_ARGS__);  \
						gz_debugbreak; \
                      } while(0)
#define gz_critical(...) GZ::Log::s_app_logger->critical(__VA_ARGS__)

#define gz_core_assert(expr, ...) do { \
                                if (!(expr)) { \
                                    GZ::Log::s_core_logger->error(__VA_ARGS__);  \
                                    gz_debugbreak; \
                                }\
                             } while(0)
#define gz_core_trace(...) GZ::Log::s_core_logger->trace(__VA_ARGS__)
#define gz_core_info(...) GZ::Log::s_core_logger->info(__VA_ARGS__)
#define gz_core_warn(...) GZ::Log::s_core_logger->warn(__VA_ARGS__)
#define gz_core_error(...) do { \
                                GZ::Log::s_core_logger->error(__VA_ARGS__);  \
                                gz_debugbreak; \
                            } while(0)
#define gz_core_critical(...) GZ::Log::s_core_logger->critical(__VA_ARGS__)
#else
#define gz_trace(...)
#define gz_info(...)
#define gz_warn(...)
#define gz_error(...)
#define gz_critical(...)

#define gz_core_trace(...)
#define gz_core_info(...)
#define gz_core_warn(...)
#define gz_core_error(...)
#define gz_core_critical(...)
#endif

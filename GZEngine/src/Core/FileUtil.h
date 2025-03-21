#pragma once
#include <string>

namespace GZ {
	struct FileUtil {
		static void get_valid_host_system_path(const char *path, std::string &out_str);
	};
}
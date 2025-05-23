#pragma once
#include <string>

namespace GZ {
	struct FileUtil {
		GZ_API static void get_valid_host_system_path(const char *path, std::string &out_str);
        GZ_API static b8 read_entire_file(const char *path, std::string &out_str);
        GZ_API static b8 save_file(const char *path, const std::string &out_str);
	};

    struct FileSystemUtil {
        GZ_API static void get_current_dir(std::string &out_dir);
    };
}

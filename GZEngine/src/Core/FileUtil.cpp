#include "FileUtil.h"

namespace GZ {

	void FileUtil::get_valid_host_system_path(const char *path, std::string &out_str) {
		// No op for OSX
#ifdef GZ_PLATFORM_WINDOWS
		out_str.clear();
		out_str.reserve(128);
		for (size_t i = 0; i < strlen(path); ++i) {
			const char &c = path[i];
			if (c == '/') {
				out_str.push_back('\\');
				out_str.push_back('\\');
			} else {
				out_str.push_back(c);
			}
		}
#endif
	}

}
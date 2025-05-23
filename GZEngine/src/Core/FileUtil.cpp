#include <SDL3/SDL.h>

#include "FileUtil.h"
#include "Log.h"

namespace GZ {

	void FileUtil::get_valid_host_system_path(const char *path, std::string &out_str) {
		// No op for OSX
        out_str.clear();
        out_str.reserve(128);
#ifdef GZ_PLATFORM_WINDOWS
		for (size_t i = 0; i < strlen(path); ++i) {
			const char &c = path[i];
			if (c == '/') {
				out_str.push_back('\\');
				out_str.push_back('\\');
			} else {
				out_str.push_back(c);
			}
		}
#else
        out_str = path;
#endif
	}

    
    b8 FileUtil::read_entire_file(const char *path, std::string &out_str) {
        out_str.clear();
        
        std::string valid_host_path;
        get_valid_host_system_path(path, valid_host_path);
        size_t byte_read = 0;
        const char *read_data = (const char *)SDL_LoadFile(valid_host_path.c_str(), &byte_read);
        if (read_data == nullptr) {
            gz_critical(SDL_GetError());
            SDL_ClearError();
            return false;
        }
        
        out_str = read_data;
        
        SDL_free((void *)read_data);
        
        return true;
    }

    b8 FileUtil::save_file(const char *path, const std::string &out_str) {
        std::string valid_host_path;
        get_valid_host_system_path(path, valid_host_path);
        if (!SDL_SaveFile(valid_host_path.c_str(), (void *)out_str.data(), out_str.size())) {
            gz_critical(SDL_GetError());
            SDL_ClearError();
            return false;
        }

        return true;
    }

    void FileSystemUtil::get_current_dir(std::string &out_dir) {
        char *read_dir = SDL_GetCurrentDirectory();
        if (read_dir == nullptr) {
            gz_critical(SDL_GetError());
            SDL_ClearError();
            return;
        }
        
        out_dir = read_dir;
        SDL_free((void *)read_dir);
    }
}

#include <gzpch.h>

#include "ProjectModule.h"

namespace GZ {
    static std::string default_project_dir = "../../LostTowerProject/";

    void ProjectModule::install_into(const ModuleContext& module_ctx) {
        cur_project_dir = default_project_dir;
    }

    const std::string &ProjectModule::get_projcet_dir() {
        return cur_project_dir;
    }

    void ProjectModule::load_project(const std::string &proj_dir) {
        cur_project_dir = proj_dir;
    }
}

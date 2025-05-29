#pragma once
#include "Module.h"
#include "ComponentInterface.h"

namespace GZ {
    struct ProjectModule final : Module {
    void install_into(const ModuleContext& module_ctx) override;
    GZ_API const std::string &get_projcet_dir();
    GZ_API void load_project(const std::string &proj_dir);

    private:
        std::string cur_project_dir;
    };
}

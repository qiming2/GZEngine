#pragma once
#include "Module.h"
#include "ComponentInterface.h"

namespace GZ {


}

namespace GZ {
    struct GameModule final : Module {
        void install_into(const ModuleContext& ctx) override;
        void uninstall_from(const ModuleContext& ctx) override;
    public:
        void create_default_game_objects();
    private:
        SceneModule *m_scene_module;
        TransformModule *m_transform_module;
    };
}

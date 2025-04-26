#pragma once
#include "Module.h"
#include "ComponentInterface.h"

namespace GZ {
    struct SceneRoot {}; // Singleton

    struct SceneModule final : Module {
        void install_into(const ModuleContext& ctx) override;
        void uninstall_from(const ModuleContext& ctx) override;
        void after_install(const ModuleContext& module_ctx) override;
        GZ_API Entity get_scene_root_entity() const;
        GZ_API Entity entity(const char *name = nullptr);
        GZ_API Entity lookup(const char *name);
        GZ_API EntityID get_scene_root_id() const;

    private:
        Entity m_scene_root;
        World *m_world; // the whole ecs
    };
}

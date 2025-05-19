#pragma once
#include "Module.h"
#include "ComponentInterface.h"

namespace GZ {
    struct SceneRoot {}; // Singleton
    struct TagComponent {
        std::string name;
    };

    struct SceneModule final : Module {
        void install_into(const ModuleContext& ctx) override;
        void uninstall_from(const ModuleContext& ctx) override;
        void after_install(const ModuleContext& module_ctx) override;
        void end_install(const ModuleContext& module_ctx) override;
        GZ_API Entity get_scene_root_entity() const;
        GZ_API Entity entity(const char *name = nullptr);
        GZ_API Entity entity(Entity parent, const char* name = nullptr);

        // Deprecated
        GZ_API Entity lookup(const char *name);
        GZ_API EntityID get_scene_root_id() const;
        GZ_API void clear_scene();
        GZ_API Entity create_new_scene();
        GZ_API Prefab get_active_scene_prefab();
        GZ_API Prefab get_active_scene();
        GZ_API Prefab prefab(Prefab parent);
        GZ_API Prefab prefab();
        GZ_API b8 load_scene(Prefab scene_prefab);

        

    private:
        Entity m_scene_root;
        Entity m_cur_scene;
        Prefab m_cur_scene_prefab;
        World *m_world; // the whole ecs
    };
}

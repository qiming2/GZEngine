#pragma once
#include "Module.h"
#include "ComponentInterface.h"
#include "Mesh.h"


#define GZ_MESH_COMPONENT_VARS(GZ_COMPONENT_TYPE_DO, GZ_COMPONENT_MEMBER_TYPE_DO, GZ_COMPONENT_TYPE_END) \
    GZ_COMPONENT_TYPE_DO(MeshComponent) \
        GZ_COMPONENT_MEMBER_TYPE_DO(std::shared_ptr<Mesh>, mesh_ref) \
    GZ_COMPONENT_TYPE_END(MeshComponent) \

namespace GZ {

    GZ_MESH_COMPONENT_VARS(GZ_COMPONENT_TYPE_DECLARE, GZ_COMPONENT_MEMBER_TYPE_DECLARE, GZ_COMPONENT_TYPE_END_DECLARE);
}

namespace GZ {
    struct RenderModule final : Module {
        void install_into(World &world, ComponentRegistry &reg) override;
        void uninstall_from(World &world, ComponentRegistry &reg) override;
    };
}

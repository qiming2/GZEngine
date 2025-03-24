#include <gzpch.h>

#include "RenderModule.h"

namespace GZ {

    void RenderModule::install_into(World &world, ComponentRegistry &reg) {
        
        GZ_MESH_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);
    }

    void RenderModule::uninstall_from(World &world, ComponentRegistry &reg) {
        
    }
}

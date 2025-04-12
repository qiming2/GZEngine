#include <gzpch.h>

#include "RenderModule.h"

namespace GZ {

    GZ_CAMERA_COMPONENT_VARS(GZ_COMPONENT_TYPE_IMPL_DRAW, GZ_COMPONENT_MEMBER_TYPE_IMPL_DRAW, GZ_COMPONENT_TYPE_END_IMPL_DRAW);

    void RenderModule::install_into(World &world, ComponentRegistry &reg) {
        
        GZ_MESH_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);
        
        GZ_CAMERA_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);
        
        GZ_CAMERA_COMPONENT_VARS(GZ_COMPONENT_TYPE_IMPL_DRAW_REG, GZ_COMPONENT_MEMBER_TYPE_IMPL_DRAW_REG, GZ_COMPONENT_TYPE_END_IMPL_DRAW_REG);
    }

    void RenderModule::uninstall_from(World &world, ComponentRegistry &reg) {
        
    }

	void RenderModule::pass_context(ModuleContext& ctx)
	{

	}

}

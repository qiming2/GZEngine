#include <gzpch.h>

#include "Renderer.h"
#include "RenderModule.h"

namespace GZ {

    GZ_CAMERA_COMPONENT_VARS(GZ_COMPONENT_TYPE_IMPL_DRAW, GZ_COMPONENT_MEMBER_TYPE_IMPL_DRAW, GZ_COMPONENT_TYPE_END_IMPL_DRAW);

    void RenderModule::install_into(const ModuleContext& module_ctx) {
        
        GZ_MESH_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);
        
        GZ_CAMERA_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);
        
        GZ_CAMERA_COMPONENT_VARS(GZ_COMPONENT_TYPE_IMPL_DRAW_REG, GZ_COMPONENT_MEMBER_TYPE_IMPL_DRAW_REG, GZ_COMPONENT_TYPE_END_IMPL_DRAW_REG);

        // Renderer needs to be inited first before RenderModule
        // After initializing the renderer, we should just render module for consistency instead
        m_renderer = module_ctx.renderer;

        m_renderer->set_transform_module(module_ctx.module_reg->get_module<TransformModule>());
    }

    void RenderModule::uninstall_from(const ModuleContext& module_ctx) {
        
    }

}

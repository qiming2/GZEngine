#pragma once
#include "Module.h"
#include "ComponentInterface.h"
#include "Mesh.h"


#define GZ_MESH_COMPONENT_VARS(GZ_COMPONENT_TYPE_DO, GZ_COMPONENT_MEMBER_TYPE_DO, GZ_COMPONENT_TYPE_END) \
    GZ_COMPONENT_TYPE_DO(MeshComponent) \
        GZ_COMPONENT_MEMBER_TYPE_DO(std::shared_ptr<Mesh>, mesh_ref) \
    GZ_COMPONENT_TYPE_END(MeshComponent) \


#define GZ_CAMERA_COMPONENT_VARS(GZ_COMPONENT_TYPE_DO, GZ_COMPONENT_MEMBER_TYPE_DO, GZ_COMPONENT_TYPE_END) \
    GZ_COMPONENT_TYPE_DO(CameraComponent) \
        GZ_COMPONENT_MEMBER_TYPE_DO(f32, fov_y) \
        GZ_COMPONENT_MEMBER_TYPE_DO(f32, aspect) \
        GZ_COMPONENT_MEMBER_TYPE_DO(f32, near) \
        GZ_COMPONENT_MEMBER_TYPE_DO(f32, far) \
        GZ_COMPONENT_MEMBER_TYPE_DO(b8, is_perspective) \
        GZ_COMPONENT_MEMBER_TYPE_DO(b8, is_primary) \
    GZ_COMPONENT_TYPE_END(CameraComponent) \

namespace GZ {

    // assume orthographic camera always pointing towards the zero
    struct CameraComponent {
        union {
            f32 viewport_w;
            f32 fov_y;
        };
        union {
            f32 viewport_h;
            f32 aspect;
        };
        f32 near = 0.1f;
        f32 far = 100.0f;
        
        b8 is_perspective = true;
        b8 is_primary = false;
        
        GZ_FORCE_INLINE mat4 get_projection_matrix() const {
            mat4 ret(1.0f);
            if (is_perspective) {
                ret = glm::perspective(fov_y, aspect, near, far);
            } else {
                ret = glm::ortho(-viewport_w / 2.0f, viewport_w / 2.0f, -viewport_h / 2.0f, viewport_h / 2.0f, 0.1f, 100.0f);
            }
            
            return ret;
        }
    };

    GZ_MESH_COMPONENT_VARS(GZ_COMPONENT_TYPE_DECLARE, GZ_COMPONENT_MEMBER_TYPE_DECLARE, GZ_COMPONENT_TYPE_END_DECLARE);
}

namespace GZ {
    struct RenderModule final : Module {
        void install_into(World &world, ComponentRegistry &reg) override;
        void uninstall_from(World &world, ComponentRegistry &reg) override;
    };
}

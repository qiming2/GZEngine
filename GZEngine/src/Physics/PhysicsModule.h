#pragma once
#include <gzpch.h>

#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/PhysicsSettings.h>

#include "CommonModule.h"
#include "ComponentInterface.h"

#define GZ_RIGIDBODY_COMPONENT_VARS(GZ_COMPONENT_TYPE_DO, GZ_COMPONENT_MEMBER_TYPE_DO, GZ_COMPONENT_TYPE_END) \
    GZ_COMPONENT_TYPE_DO(RigidbodyComponent) \
        GZ_COMPONENT_MEMBER_TYPE_DO(JPH::BodyID, id) \
    GZ_COMPONENT_TYPE_END(RigidbodyComponent) \

#define GZ_PREV_TRANSFORM_COMPONENT_VARS(GZ_COMPONENT_TYPE_DO, GZ_COMPONENT_MEMBER_TYPE_DO, GZ_COMPONENT_TYPE_END) \
    GZ_COMPONENT_TYPE_DO(PrevTransformComponent) \
        GZ_COMPONENT_MEMBER_TYPE_DO(vec3, p) \
        GZ_COMPONENT_MEMBER_TYPE_DO(quat, r) \
    GZ_COMPONENT_TYPE_END(PrevTransformComponent) \

namespace GZ {

    GZ_RIGIDBODY_COMPONENT_VARS(GZ_COMPONENT_TYPE_DECLARE, GZ_COMPONENT_MEMBER_TYPE_DECLARE, GZ_COMPONENT_TYPE_END_DECLARE);

    GZ_PREV_TRANSFORM_COMPONENT_VARS(GZ_COMPONENT_TYPE_DECLARE, GZ_COMPONENT_MEMBER_TYPE_DECLARE, GZ_COMPONENT_TYPE_END_DECLARE);

    struct PhysicsModule : Module {
    public: // Module interface
        void install_into(World &world, ComponentRegistry &reg) override;
        void uninstall_from(World &world, ComponentRegistry &reg) override;
    private: // system queries
        flecs::query<const TransformComponent, const RigidbodyComponent, const PrevTransformComponent> q;
        flecs::query<TransformComponent, const RigidbodyComponent, PrevTransformComponent> q1;
    public:
        b8 init();
    
        // Temporary
        void create_default_objects();
    
        void simulate(f32 delta_time, World &world);
    
        void deinit();
    public: // Test and hack things together
        vec3 get_sphere_position();
        vec3 get_box_position();
         JPH::BodyID m_box_id;
         JPH::BodyID m_sphere_id;
    private: // temp testing objects
        // Remove the sphere from the physics system. Note that the sphere itself keeps all of its state and can be re-added at any time.
        void destroy_default_objects();
        JPH::BodyID m_floor_id;
    
    private: // runtime use
        f32 m_accumulated = 0.0f;
    private: // init setup
        b8 m_is_initialized = false;
        JPH::BroadPhaseLayerInterface *m_broad_phase_layer_interface;
        JPH::ObjectVsBroadPhaseLayerFilter *m_object_vs_broadphase_layer_filter;
        JPH::ObjectLayerPairFilter *m_object_vs_object_layer_filter;
        JPH::BodyActivationListener *m_body_activation_listener;
        JPH::ContactListener *m_contact_listener;
        JPH::PhysicsSystem m_physics_system;
        JPH::JobSystem *m_job_system;
        JPH::TempAllocator *m_temp_allocator;
        JPH::BodyInterface *m_body_interface;
        // Default to 1 / 60 secs per tick
        f32 m_simulation_step_time = 1.0f / 60.0f;
        i32 m_collision_step_per_simulate_step = 1;
    };

}

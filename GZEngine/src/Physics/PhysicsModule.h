#pragma once
#include <gzpch.h>

#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <Jolt/Renderer/DebugRenderer.h>
#include <Jolt/Math/Real.h>
#include <Jolt/Math/Quat.h>

#include "TransformModule.h"
#include "ComponentInterface.h"

#define GZ_RIGIDBODY_COMPONENT_VARS(GZ_COMPONENT_TYPE_DO, GZ_COMPONENT_MEMBER_TYPE_DO, GZ_COMPONENT_TYPE_END) \
    GZ_COMPONENT_TYPE_DO(RigidbodyComponent) \
        GZ_COMPONENT_MEMBER_TYPE_DO(JPH::BodyID, id) \
    GZ_COMPONENT_TYPE_END(RigidbodyComponent) \

#define GZ_CHARACTER_COMPONENT_VARS(GZ_COMPONENT_TYPE_DO, GZ_COMPONENT_MEMBER_TYPE_DO, GZ_COMPONENT_TYPE_END) \
    GZ_COMPONENT_TYPE_DO(CharacterComponent) \
        GZ_COMPONENT_MEMBER_TYPE_DO(vec3, vel) \
    GZ_COMPONENT_TYPE_END(CharacterComponent) \

namespace GZ {
    
    GZ_RIGIDBODY_COMPONENT_VARS(GZ_COMPONENT_TYPE_DECLARE, GZ_COMPONENT_MEMBER_TYPE_DECLARE, GZ_COMPONENT_TYPE_END_DECLARE);

    GZ_CHARACTER_COMPONENT_VARS(GZ_COMPONENT_TYPE_DECLARE, GZ_COMPONENT_MEMBER_TYPE_DECLARE, GZ_COMPONENT_TYPE_END_DECLARE);

    struct PhysicsModule : Module {
	public: // Module interface
		void install_into(const ModuleContext &module_ctx) override;
        void after_install(const ModuleContext &module_ctx) override;
        void uninstall_from(const ModuleContext&module_ctx) override;
    public: // Tweakable vals, intend for quick debugging and testing, not really for shipping
		b8 is_physics_debug_on = false;
    private: // system queries
        
    public:
        b8 init(World &world);
    
        // Temporary
        void create_default_objects();

		// Remove the sphere from the physics system. Note that the sphere itself keeps all of its state and can be re-added at any time.
		void destroy_default_objects();
    
        void deinit();
    public: // Test and hack things together
        JPH::BodyID m_box_id;
        JPH::BodyID m_sphere_id;
        JPH::BodyID m_floor_id;
        JPH::BodyID m_character_id;
    private: // Character stuff
        JPH::CharacterVirtual *m_main_character;
    public: // runtime use, this is useful to expose
        f32 m_accumulated = 0.0f;
        size_t m_num_physics_ticks_cur_frame = 0;

		// Default to 1 / 60 secs per tick
		f32 m_simulation_step_time = 1.0f / 90.0f;
		i32 m_collision_step_per_simulate_step = 1;
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
        
    private: // Physic debug usage
        JPH::DebugRenderer *m_physics_renderer;
    };

}

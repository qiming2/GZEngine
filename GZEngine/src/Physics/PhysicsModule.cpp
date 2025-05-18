#include <PhysicsModule.h>

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Renderer/DebugRendererSimple.h>

#include "CommonModule.h"
#include "Log.h"
#include "MathUtil.h"
#include "RenderModule.h"
#include "Renderer.h"
#include "GameModule.h"
#include "Profiler.h"

#define gz_physics_scoped_profiler() gz_scoped_profiler("Physics")
namespace GZ {
    using namespace JPH;

    // Layer that objects can be in, determines which other objects it can collide with
    // Typically you at least want to have 1 layer for moving bodies and 1 layer for static bodies, but you can have more
    // layers if you want. E.g. you could have a layer for high detail collision (which is not used by the physics simulation
    // but only if you do collision testing).
    namespace Layers
    {
        static constexpr ObjectLayer NON_MOVING = 1 << 0;
        static constexpr ObjectLayer CHARACTER = 1 << 1;
        static constexpr ObjectLayer MOVING = 1 << 2;

        static constexpr ObjectLayer NUM_LAYERS = 3;

        // Convenience layer decl
        static constexpr ObjectLayer ALL_MOVING_OBJECTS = CHARACTER | MOVING;
        static constexpr ObjectLayer ALL_OBJECTS = ObjectLayer(~0);
    };

    /// Class that determines if two object layers can collide
    class ObjectLayerPairFilterImpl : public ObjectLayerPairFilter
    {
    public:
        virtual b8 ShouldCollide(ObjectLayer inObject1, ObjectLayer inObject2) const override
        {
            switch (inObject1)
            {
            case Layers::NON_MOVING:
                return inObject2 & Layers::ALL_MOVING_OBJECTS; // Non moving only collides with moving
            case Layers::MOVING:
                return inObject2 & Layers::ALL_OBJECTS; // Moving collides with everything
            case Layers::CHARACTER:
                return inObject2 & Layers::ALL_OBJECTS;
            default:
                gz_error("Non defined object layer");
                return false;
            }
        }
    };

    // Each broadphase layer results in a separate bounding volume tree in the broad phase. You at least want to have
    // a layer for non-moving and moving objects to avoid having to update a tree full of static objects every frame.
    // You can have a 1-on-1 mapping between object layers and broadphase layers (like in this case) but if you have
    // many object layers you'll be creating many broad phase trees, which is not efficient. If you want to fine tune
    // your broadphase layers define JPH_TRACK_BROADPHASE_STATS and look at the stats reported on the TTY.
    namespace BroadPhaseLayers
    {
        static constexpr BroadPhaseLayer NON_MOVING(0);
        static constexpr BroadPhaseLayer MOVING(1);
        static constexpr uint NUM_LAYERS(2);
    };

    // BroadPhaseLayerInterface implementation
    // This defines a mapping between object and broadphase layers.
    class BPLayerInterfaceImpl final : public BroadPhaseLayerInterface
    {
    public:
        BPLayerInterfaceImpl()
        {

        }

        virtual uint GetNumBroadPhaseLayers() const override
        {
            return BroadPhaseLayers::NUM_LAYERS;
        }

        virtual BroadPhaseLayer GetBroadPhaseLayer(ObjectLayer inLayer) const override
        {
            /*JPH_ASSERT(inLayer < Layers::NUM_LAYERS);*/
			switch (inLayer)
			{
            case Layers::NON_MOVING:    return BroadPhaseLayers::NON_MOVING;
			case Layers::MOVING:        return BroadPhaseLayers::MOVING;
			case Layers::CHARACTER:     return BroadPhaseLayers::MOVING;
            default: gz_error("Layer not defined {}", static_cast<uint32_t>(inLayer)); return BroadPhaseLayers::NON_MOVING;
			}
        }

    #if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
        virtual const char *GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const override
        {
            switch ((BroadPhaseLayer::Type)inLayer)
            {
            case (BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:    return "NON_MOVING";
            case (BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:        return "MOVING";
            default: gz_error("Layer not defined {}", static_cast<uint8_t>(inLayer));         return "INVALID";
            }
        }
    #endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED
    };


    // An example activation listener
    class MyBodyActivationListener : public BodyActivationListener
    {
    public:
        virtual void OnBodyActivated(const BodyID &inBodyID, uint64 inBodyUserData) override
        {
            //gz_info("Body: [{}] activated", inBodyID.GetIndexAndSequenceNumber());
        }

        virtual void OnBodyDeactivated(const BodyID &inBodyID, uint64 inBodyUserData) override
        {
            //gz_info("Body: [{}] deactivated", inBodyID.GetIndexAndSequenceNumber());
        }
    };

    /// Class that determines if an object layer can collide with a broadphase layer
    class ObjectVsBroadPhaseLayerFilterImpl : public ObjectVsBroadPhaseLayerFilter
    {
    public:
        virtual b8 ShouldCollide(ObjectLayer inLayer1, BroadPhaseLayer inLayer2) const override
        {
            switch (inLayer1)
            {
            case Layers::NON_MOVING:
                return inLayer2 == BroadPhaseLayers::MOVING;
            case Layers::MOVING:
                return true;
            case Layers::CHARACTER:
                return true;
            default:
                gz_error("Layer not defined {}", inLayer1);
                return false;
            }
        }
    };

    // An example contact listener
    class MyContactListener : public ContactListener
    {
    public:
        MyContactListener(World& world) {
            m_world = &world;
        }
    public:
        // See: ContactListener
        virtual ValidateResult OnContactValidate(const Body &inBody1, const Body &inBody2, RVec3Arg inBaseOffset, const CollideShapeResult &inCollisionResult) override
        {
            //gz_info("Contact validate callback");
            
            // Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
            return ValidateResult::AcceptAllContactsForThisBodyPair;
        }

		virtual void OnContactAdded(const Body& inBody1, const Body& inBody2, const ContactManifold& inManifold, ContactSettings& ioSettings) override
        {
            // we need another rigidbodycomponent on removed hook, otherwise invisible objects are still moving
			/*if (!m_world->entity(static_cast<flecs::entity_t>(inBody1.GetUserData())).is_alive() || !m_world->entity(static_cast<flecs::entity_t>(inBody2.GetUserData())).is_alive() ) return;

			gz_info("Body {} has contacts with Body {}", m_world->entity(static_cast<flecs::entity_t>(inBody1.GetUserData())).name().c_str(), m_world->entity(static_cast<flecs::entity_t>(inBody2.GetUserData())).name().c_str());

			m_world->entity(static_cast<flecs::entity_t>(inBody1.GetUserData())).destruct();*/
        }

        virtual void OnContactPersisted(const Body &inBody1, const Body &inBody2, const ContactManifold &inManifold, ContactSettings &ioSettings) override
        {
            //gz_info("Body {} has contacts persisted with Body {}", inBody1.GetID().GetIndexAndSequenceNumber(), inBody2.GetID().GetIndexAndSequenceNumber());
        }

        virtual void OnContactRemoved(const SubShapeIDPair &inSubShapePair) override
        {
            //gz_info("Body {} has contacts removed with Body {}", inSubShapePair.GetBody1ID().GetIndexAndSequenceNumber(), inSubShapePair.GetBody2ID().GetIndexAndSequenceNumber());
        }
    private:
        // We should store an array of contacts here
        // resolve them before next game update
        
        World *m_world;
    };


	GZ_FORCE_INLINE vec3 to_glm(const RVec3& jolt_val) {
		return vec3(jolt_val.GetX(), jolt_val.GetY(), jolt_val.GetZ());
	}

	GZ_FORCE_INLINE quat to_glm(const Quat& jolt_val) {
		return quat(jolt_val.GetW(), jolt_val.GetX(), jolt_val.GetY(), jolt_val.GetZ());
	}

	GZ_FORCE_INLINE RVec3 to_jolt(const vec3& glm_val) {
		return RVec3(glm_val.x, glm_val.y, glm_val.z);
	}

	GZ_FORCE_INLINE Quat to_jolt(const quat& glm_val) {
		return Quat(glm_val.x, glm_val.y, glm_val.z, glm_val.w);
	}

    class PhysicsDebugRenderer : public DebugRendererSimple {
    public:
        PhysicsDebugRenderer(Renderer* renderer) {
            m_renderer = renderer;
        }
    public:
        virtual void DrawLine(RVec3Arg from, RVec3Arg to, ColorArg color) override {
            gz_info("Physics Draw line Not implemented");
        }

        virtual void DrawTriangle(RVec3Arg inV1, RVec3Arg inV2, RVec3Arg inV3, ColorArg inColor, ECastShadow inCastShadow) override {
            Vec4 color = inColor.ToVec4();
            m_renderer->physics_debug_draw_triangle(to_glm(inV1), to_glm(inV2), to_glm(inV3), vec3{color.GetX(), color.GetY(), color.GetZ()});
        }

        virtual void DrawText3D(JPH::RVec3Arg inPosition, const string_view& inString, JPH::ColorArg inColor, float inHeight) override {
            gz_info("Text not implemented");
        }
    private:
        Renderer *m_renderer;
    };

    // Callback for traces, connect this to your own trace function if you have one
    static void jolt_trace_impl(const char *inFMT, ...)
    {
        // Format the message
        va_list list;
        va_start(list, inFMT);
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), inFMT, list);
        va_end(list);

        // Print to the TTY
        gz_warn("[PHYSICS TRACE]: {}", buffer);
    }

    #ifdef JPH_ENABLE_ASSERTS
    static b8 jolt_assert_failed_impl(const char *inExpression, const char *inMessage, const char *inFile, uint inLine) {
        gz_error("PHYSICS ASSERT FAILED: in {}, error: {}, File_Line: {}:{}", inExpression, inMessage, inFile, inLine);
        return false;
    }
    #endif

    GZ_CHARACTER_COMPONENT_VARS(GZ_COMPONENT_TYPE_IMPL_DRAW, GZ_COMPONENT_MEMBER_TYPE_IMPL_DRAW, GZ_COMPONENT_TYPE_END_IMPL_DRAW);

	void PhysicsModule::install_into(const ModuleContext &module_ctx)
	{
        World &world = *module_ctx.world;
        ComponentRegistry &reg = *module_ctx.reg;
        if (!init(world))
            gz_error("Something wrong!");

        m_physics_renderer = new PhysicsDebugRenderer(module_ctx.renderer);
        // System component initialization
        world.component<JPH::BodyID>()
            .member<u32>("mID");

        GZ_RIGIDBODY_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);

        GZ_CHARACTER_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);
        
        world.component<RigidbodyComponent>().add(flecs::Exclusive);
        world.component<CharacterComponent>().add(flecs::Exclusive);

		// Calculate num physics ticks need to happen in current frame, this is needed since
		// Jolt's character and physics world update are separate, and we want to take advantage of multithreaded system update
		// so we pre calculate num ticks here
		System pre_physics_update = world.system("pre_physics_update")
			.kind(flecs::OnUpdate)
			.run([&](WorldIter& it) {

			m_num_physics_ticks_cur_frame = 0;
			m_accumulated += it.delta_time();
			m_num_physics_ticks_cur_frame = static_cast<size_t>(m_accumulated / m_simulation_step_time);
			m_accumulated = std::fmodf(m_accumulated, m_simulation_step_time);
		});

        // Create bodies and characters
        System update_dirty_rigidbody_components = world.system<const TransformComponent, const RigidbodyComponent>("update_dirty_rigidbody_components")
            .term_at(1).second<DirtyTrait>()
            .kind(flecs::OnUpdate)
            .write<RigidbodyComponent>()
            .each([&](flecs::entity e, const TransformComponent &t_comp, const RigidbodyComponent &r_comp) {
            gz_info("Create bodies");
            
            m_body_interface->SetUserData(r_comp.id, static_cast<u64>(e.id()));
            e.remove<RigidbodyComponent, DirtyTrait>();
        });
        
		System update_dirty_character_components = world.system<const TransformComponent, const CharacterComponent>("update_dirty_character_components")
			.term_at(1).second<DirtyTrait>()
			.kind(flecs::OnUpdate)
            .write<CharacterComponent>()
			.each([&](Entity e, const TransformComponent& t_comp, const CharacterComponent& r_comp) {
			gz_info("Create Characters bodies");

            e.set<CharacterComponent>(r_comp);
            m_main_character->SetUserData(e.id());
            e.remove<CharacterComponent, DirtyTrait>();
		});


        // TODO(Qiming): get world transform instead of local
        // Systems can be multithreaded, so we could take advantage of this
        System ecs_to_sim = world.system<const TransformComponent, const RigidbodyComponent>("ecs_to_sim")
            .kind(flecs::OnUpdate)
            .multi_threaded()
            .each([&](Entity e, const TransformComponent& transform, const RigidbodyComponent& rigidbody) {
             m_body_interface->SetPositionAndRotationWhenChanged(rigidbody.id, to_jolt(transform.p), to_jolt(glm::normalize(transform.r)), EActivation::Activate);
        });

        // This is only for characters
		static vec3 prev_p = { 0, 0, 0 };
		static vec3 next_p = { 0, 0, 0 };
        static vec3 cached_cur_frame = {0, 0, 0};
		System ecs_to_sim_char = world.system<const TransformComponent, const CharacterComponent>("ecs_to_sim_char")
			.kind(flecs::OnUpdate)
			.multi_threaded()
			.each([&](WorldIter& it, size_t index, const TransformComponent& transform, const CharacterComponent& char_comp) {
            if (!is_vec3_all_equal_epsilon(cached_cur_frame, transform.p)) {
                prev_p = next_p = transform.p;
                m_main_character->SetPosition(to_jolt(transform.p));
            }
			else if (!it.entity(index).has<Player>()) {
				m_main_character->SetPosition(to_jolt(transform.p));
			}
			
			m_main_character->SetRotation(to_jolt(glm::normalize(transform.r)));
			m_main_character->SetLinearVelocity(to_jolt(char_comp.vel));
		});

		// Character is also a physics update, so we need to run in phyiscs update system
		System character_update = world.system("character_update")
			.kind(flecs::OnUpdate)
			.run([&](WorldIter &it) {
            gz_physics_scoped_profiler();
			size_t num_ticks = m_num_physics_ticks_cur_frame;
			while (num_ticks) {
				// Character update
				//m_main_character->UpdateGroundVelocity();
				// Settings for our update function
				//CharacterVirtual::ExtendedUpdateSettings update_settings;

				////// Update the character position
				//m_main_character->ExtendedUpdate(m_simulation_step_time,
				//	-m_main_character->GetUp() * m_physics_system.GetGravity().Length(),
				//	update_settings,
				//	m_physics_system.GetDefaultBroadPhaseLayerFilter(Layers::CHARACTER),
				//	m_physics_system.GetDefaultLayerFilter(Layers::CHARACTER),
				//	{ },
				//	{ },
				//	*m_temp_allocator);

                m_main_character->Update(m_simulation_step_time, {0, 0, 0}, m_physics_system.GetDefaultBroadPhaseLayerFilter(Layers::CHARACTER), m_physics_system.GetDefaultLayerFilter(Layers::CHARACTER), {}, {}, *m_temp_allocator);
				num_ticks--;
			}


            // the main character update once per frame
            //m_main_character->Update(it.delta_time(), {0, 0, 0}, m_physics_system.GetDefaultBroadPhaseLayerFilter(Layers::CHARACTER), m_physics_system.GetDefaultLayerFilter(Layers::CHARACTER), {}, {}, *m_temp_allocator);

		});

		System physics_update = world.system("physics_update")
            .kind(flecs::OnUpdate)
			.run([&](WorldIter& it) {
            gz_physics_scoped_profiler();
            size_t num_ticks = m_num_physics_ticks_cur_frame;
            while (num_ticks) {
                m_physics_system.Update(m_simulation_step_time, m_collision_step_per_simulate_step, m_temp_allocator, m_job_system);
                num_ticks--;
            }
		});

		
		System sim_to_ecs_char = world.system<TransformComponent, CharacterComponent>("sim_to_ecs_char")
			.kind(flecs::OnUpdate)
			.multi_threaded()
			.each([&](WorldIter& it, size_t index, TransformComponent& transform, CharacterComponent& char_comp) {

            Entity e = it.entity(index);
            if (m_num_physics_ticks_cur_frame && e.has<Player>()) {
                prev_p = next_p;
                next_p = to_glm(m_main_character->GetPosition());
            }

            if (e.has<Player>()) {
				cached_cur_frame = glm::mix(prev_p, next_p, m_accumulated / m_simulation_step_time);
				transform.p = cached_cur_frame;
            }
            else {
                transform.p = to_glm(m_main_character->GetPosition());
            }

			transform.r = glm::normalize(to_glm(m_main_character->GetRotation()));
			char_comp.vel = to_glm(m_main_character->GetLinearVelocity());
            e.modified<TransformComponent>();
		});

		System sim_to_ecs = world.system<TransformComponent, const RigidbodyComponent>("sim_to_ecs")
            .kind(flecs::OnUpdate)
            .multi_threaded()
			.each([&](WorldIter& it, size_t index, TransformComponent& transform, const RigidbodyComponent& rigidbody) {
			transform.p = to_glm(m_body_interface->GetPosition(rigidbody.id));
			transform.r = glm::normalize(to_glm(m_body_interface->GetRotation(rigidbody.id)));
            it.entity(index).modified<TransformComponent>();
		});

		System physics_debug_draw = world.system("physics_debug_draw")
			.kind(flecs::OnUpdate)
			.run([&](WorldIter& it) {
            if (is_physics_debug_on)
                m_physics_system.DrawBodies(BodyManager::DrawSettings(), m_physics_renderer);
		});
	}

	void PhysicsModule::after_install(const ModuleContext& module_ctx)
	{
        create_default_objects();
	}

	void PhysicsModule::uninstall_from(const ModuleContext& ctx)
	{
        destroy_default_objects();
	}

	// Basic setup following official jolt helloworld, with slight change
	b8 PhysicsModule::init(World &world) {
        gz_info("Init Physics...");
        
        // Can have custom allocator
        RegisterDefaultAllocator();
        
        // Install trace and assert callbacks
        Trace = jolt_trace_impl;
        JPH_IF_ENABLE_ASSERTS(AssertFailed = jolt_assert_failed_impl;)
        
        // For jolt creation of its classes, required
        Factory::sInstance = new Factory();

        // Register all physics types with the factory and install their collision handlers with the CollisionDispatch class.
        // If you have your own custom shape types you probably need to register their handlers with the CollisionDispatch before calling this function.
        // If you implement your own default material (PhysicsMaterial::sDefault) make sure to initialize it before this function or else this function will create one for you.
        RegisterTypes();
        
        // We need a temp allocator for temporary allocations during the physics update. We're
        // pre-allocating 10 MB to avoid having to do allocations during the physics update.
        // B.t.w. 10 MB is way too much for this example but it is a typical value you can use.
        // If you don't want to pre-allocate you can also use TempAllocatorMalloc to fall back to
        // malloc / free.
        m_temp_allocator = new TempAllocatorImpl(10 * 1024 * 1024);
        
        // We need a job system that will execute physics jobs on multiple threads. Typically
        // you would implement the JobSystem interface yourself and let Jolt Physics run on top
        // of your own job scheduler. JobSystemThreadPool is an example implementation.
        m_job_system = new JobSystemThreadPool(cMaxPhysicsJobs, cMaxPhysicsBarriers, thread::hardware_concurrency() - 1);
        
        // This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
        // Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
        // Make it 65536, I doubt lost tower would use many but so we want don't have to worry about it
        const uint cMaxBodies = 65536;
        
        // This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
        // Set it to zero since we don't need concurrent access
        // Update loop is only happening on main thread
        const uint cNumBodyMutexes = 0;
        
        // This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
        // body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
        // too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
        // Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
        const uint cMaxBodyPairs = 65536;
        
        // This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
        // number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
        // Note: This value is low because this is a simple test. For a real project use something in the order of 10240.
        // Make it high for contact
        const uint cMaxContactConstraints = 10240;
        
        // Create mapping table from object layer to broadphase layer
        // Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
        // Also have a look at BroadPhaseLayerInterfaceTable or BroadPhaseLayerInterfaceMask for a simpler interface.
        m_broad_phase_layer_interface = new BPLayerInterfaceImpl();
        
        // Create class that filters object vs broadphase layers
        // Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
        // Also have a look at ObjectVsBroadPhaseLayerFilterTable or ObjectVsBroadPhaseLayerFilterMask for a simpler interface.
        m_object_vs_broadphase_layer_filter = new ObjectVsBroadPhaseLayerFilterImpl();
        
        // Create class that filters object vs object layers
        // Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
        // Also have a look at ObjectLayerPairFilterTable or ObjectLayerPairFilterMask for a simpler interface.
        m_object_vs_object_layer_filter = new ObjectLayerPairFilterImpl();
        
        // Now we can create the actual physics system.
        m_physics_system.Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, *m_broad_phase_layer_interface, *m_object_vs_broadphase_layer_filter, *m_object_vs_object_layer_filter);

        // A body activation listener gets notified when bodies activate and go to sleep
        // Note that this is called from a job so whatever you do here needs to be thread safe.
        // Registering one is entirely optional.
        m_body_activation_listener = new MyBodyActivationListener();
        m_physics_system.SetBodyActivationListener(m_body_activation_listener);
        
        // A contact listener gets notified when bodies (are about to) collide, and when they separate again.
        // Note that this is called from a job so whatever you do here needs to be thread safe.
        // Registering one is entirely optional.
        m_contact_listener = new MyContactListener(world);
        m_physics_system.SetContactListener(m_contact_listener);
        
        // The main way to interact with the bodies in the physics system is through the body interface. There is a locking and a non-locking
        // variant of this. We're going to use the locking version (even though we're not planning to access bodies from multiple threads)
		// We don't need locking
		m_body_interface = &(m_physics_system.GetBodyInterfaceNoLock());
        
        // No we can start creating some physics body
        m_is_initialized = true;
		return true;
	}

    void PhysicsModule::create_default_objects() {
        gz_core_assert(m_is_initialized, "Physics is not initialized!");

        // Next we can create a rigid body to serve as the floor, we make a large box
        // Create the settings for the collision volume (the shape).
        // Note that for simple shapes (like boxes) you can also directly construct a BoxShape.
        BoxShapeSettings floor_shape_settings(Vec3(100.0f, 1.0f, 100.0f));
        floor_shape_settings.SetEmbedded(); // A ref counted object on the stack (base class RefTarget) should be marked as such to prevent it from being freed when its reference count goes to 0.

        // Create the shape
        ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
        ShapeRefC floor_shape = floor_shape_result.Get(); // We don't expect an error here, but you can check floor_shape_result for HasError() / GetError()

        // Create the settings for the body itself. Note that here you can also set other properties like the restitution / friction.
        BodyCreationSettings floor_settings(floor_shape, RVec3(0.0f, -2.0f, 0.0f), Quat::sIdentity(), EMotionType::Static, Layers::NON_MOVING);

        // Create the actual rigid body
        Body *floor = m_body_interface->CreateBody(floor_settings); // Note that if we run out of bodies this can return nullptr
        m_floor_id = floor->GetID();
        // Add it to the world
        m_body_interface->AddBody(floor->GetID(), EActivation::DontActivate);

        // Now create a dynamic body to bounce on the floor
        // Note that this uses the shorthand version of creating and adding a body to the world
        BodyCreationSettings sphere_settings(new SphereShape(0.5f), RVec3(-1.0f, 1.0f, 0.0f), Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING);
        //sphere_settings.mGravityFactor = 1.0f; // We do our own gravity
        //sphere_settings.mOverrideMassProperties = EOverrideMassProperties::CalculateMassAndInertia;
//        sphere_settings.mMassPropertiesOverride.mMass = EOverrideMassProperties::cal;
        /*sphere_settings.mAngularDamping = 0.5f;*/
        
        m_sphere_id = m_body_interface->CreateAndAddBody(sphere_settings, EActivation::Activate);
        //m_body_interface->SetLinearVelocity(m_sphere_id, Vec3(0.0f, 0.0f, -5.0f));
        m_body_interface->AddImpulse(m_sphere_id, {0.0f, 10000.0f, 0.0f});
        m_body_interface->SetRestitution(m_sphere_id, 1.0f);


        BodyCreationSettings box_settings(new BoxShape({0.5f, 0.5f, 0.5f}), RVec3(3.0f, 2.0f, 0.0f), Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING);
        m_box_id = m_body_interface->CreateAndAddBody(box_settings, EActivation::Activate);
        m_body_interface->SetLinearVelocity(m_box_id, {-1.0f, 1.0f, -1.0f});
        m_body_interface->SetAngularVelocity(m_box_id, {0.0f, 10.0f, 0.0f});
        m_body_interface->SetRestitution(m_box_id,0.8f);
        m_body_interface->SetFriction(m_box_id, 0.01f);
        // Now you can interact with the dynamic body, in this case we're going to give it a velocity.
        // (note that if we had used CreateBody then we could have set the velocity straight on the body before adding it to the physics system)

        f32 character_height = 0.5f;
        f32 character_radius = 0.25;
        f32 character_inner_fraction = 1.0f;
		CharacterVirtualSettings settings;
		auto mStandingShape = RotatedTranslatedShapeSettings(Vec3(0, 0.5f * character_height + character_radius, 0), Quat::sIdentity(), new CapsuleShape(0.5f * character_height, character_radius)).Create().Get();

		auto mInnerStandingShape = RotatedTranslatedShapeSettings(Vec3(0, 0.5f * character_height + character_radius, 0), Quat::sIdentity(), new CapsuleShape(0.5f * character_inner_fraction * character_height, character_inner_fraction* character_radius)).Create().Get();

		settings.mShape = mStandingShape;
		settings.mInnerBodyShape = mInnerStandingShape;
        settings.mInnerBodyLayer = Layers::CHARACTER;
		settings.mSupportingVolume = Plane(Vec3::sAxisY(), -character_radius); // Accept contacts that touch the lower sphere of the capsule
		m_main_character = new CharacterVirtual(&settings, RVec3(0.0f, 0.0f, 0.0f), Quat::sIdentity(), uint64(0), &m_physics_system);
        
        gz_info("Character inner body id {}", m_main_character->GetInnerBodyID().GetIndexAndSequenceNumber());
        // Get ids from character, most likely we are just going to use inner rigidbody id,
        /*mAnimatedCharacterVirtualWithInnerBody->GetInnerBodyID();
        mAnimatedCharacterVirtualWithInnerBody->GetID();
        mAnimatedCharacterVirtualWithInnerBody->SetUserData(0);*/

        

        // We don't need to char vs char for now, I think we don't even need character virutal since we are top down with no ramp stuff like that,
        // kinematic object should just suffice the need, but we can practice using this class so if in the future we want to make a first person and open world rpg,
        // the knowledge can be handy.
		/*
        static auto m_contact_char = CharacterVsCharacterCollisionSimple();
        mAnimatedCharacterVirtualWithInnerBody->SetCharacterVsCharacterCollision(&m_contact_char);
		m_contact_char.Add(mAnimatedCharacterVirtualWithInnerBody);
        */

    }

    /*vec3 position = to_glm(m_body_interface->GetPosition(m_box_id));
            vec3 velocity = to_glm(m_body_interface->GetLinearVelocity(m_sphere_id));*/
            
    // If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable. Do 1 collision step per 1 / 60th of a second (round up).
    //gz_info("Sphere: pos: {}, vel: {}", position, velocity);

    void PhysicsModule::destroy_default_objects() {
        m_body_interface->RemoveBody(m_sphere_id);
        m_body_interface->DestroyBody(m_sphere_id);
        m_body_interface->RemoveBody(m_floor_id);
        m_body_interface->DestroyBody(m_floor_id);
        delete m_main_character;
    }

    void PhysicsModule::deinit() {
        delete m_physics_renderer;
        delete m_broad_phase_layer_interface;
        delete m_object_vs_broadphase_layer_filter;
        delete m_object_vs_object_layer_filter;
        delete m_body_activation_listener;
        delete m_contact_listener;
        delete m_temp_allocator;
        delete m_job_system;
        
        UnregisterTypes();
        delete Factory::sInstance;
        Factory::sInstance = nullptr;
    }
}

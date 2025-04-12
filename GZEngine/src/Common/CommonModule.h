#pragma once
#include "Module.h"
#include "Log.h"
#include "ComponentInterface.h"

////////////////////// Some examples of common components //////////////////////////////

// Declaration order does not matter but Registratin order in module install_into does matter like how c works

// rotation is x, y, z axis
#define GZ_TRANSFORM_COMPONENT_VARS(GZ_COMPONENT_TYPE_DO, GZ_COMPONENT_MEMBER_TYPE_DO, GZ_COMPONENT_TYPE_END) \
	GZ_COMPONENT_TYPE_DO(TransformComponent) \
		GZ_COMPONENT_MEMBER_TYPE_DO(vec3, p) \
		GZ_COMPONENT_MEMBER_TYPE_DO(quat, r) \
		GZ_COMPONENT_MEMBER_TYPE_DO(vec3, s) \
	GZ_COMPONENT_TYPE_END(TransformComponent) \

namespace GZ {
    // Specifically editor stuff
    struct EditorTrait{};

    struct DirtyTrait{};
    struct LoadingTrait{};
    struct ReadyTrait{};
	// declaration order does not matter

	// Many vec types already declared and defined, but not registered in world!
	// vec3 is already defined, we don't need to do these again
    

//	GZ_TRANSFORM_COMPONENT_VARS(GZ_COMPONENT_TYPE_DECLARE, GZ_COMPONENT_MEMBER_TYPE_DECLARE, GZ_COMPONENT_TYPE_END_DECLARE);

    struct TransformComponent {
        vec3 p = vec3{0, 0, 0};
        quat r = quat(1, 0, 0, 0);
        vec3 s = vec3(1, 1, 1);
        
        GZ_FORCE_INLINE mat4 get_model_matrix() const {
            return glm::translate(mat4(1.0f), p) * glm::mat4_cast(glm::normalize(r)) * glm::scale(mat4(1.0f), s);
        }
    };
}

// Component formatter
template<>
struct fmt::formatter<GZ::TransformComponent> : fmt::formatter<std::string>
{
	auto format(GZ::TransformComponent my_transform, format_context& ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "\np: {}\ns: {}\nr: {}", my_transform.p, my_transform.r, my_transform.s);
	}
};

namespace GZ {
	// Expose this for each ecs module
	// register components, systems, triggers, hooks all here pre ticking the game
	struct CommonModule final : Module {
		void install_into(World &world, ComponentRegistry &reg) override;
		void uninstall_from(World &world, ComponentRegistry &reg) override;
		void pass_context(ModuleContext &ctx) override;
	};
}

// ------------------------- Sync point examples --------


//        struct TestComponent {
//            std::string test = "YoYo";
//        };
//        world.component<TestComponent>().add(flecs::Exclusive);
//
//        auto test_ent = world.entity().set<TestComponent, DirtyTrait>({"ssdasdqw"});
//
//        world.system<const TestComponent>("test_dirty")
//            .term_at(0).second<DirtyTrait>()
//            .write<TestComponent, ReadyTrait>()
//            .kind(flecs::OnUpdate)
//            .each([&](const Entity &ent, const TestComponent &test_comp) {
//                ent.set<TestComponent, ReadyTrait>({"Ready to roll!"});
//                ent.set<vec3>({});
//                gz_info("With dirty matching {}", test_comp.test);
//        });
//
//        System physics_update = world.system<const TestComponent>("test_update")
//            .kind(flecs::OnUpdate)
//            .run([=](WorldIter& it) {
//                gz_info("Hello test");
//                if (test_ent.has<TestComponent, ReadyTrait>()) {
//                    gz_info("Has readytrait");
//                } else {
//                    gz_info("Does not have readytrait");
//                }
//
//                if (test_ent.has<vec3>()) {
//                    gz_info("has vec3");
//                }
//        });
//
//        world.system<const TestComponent>("test")
//            .term_at(0).second<ReadyTrait>()
//            .multi_threaded()
//            .kind(flecs::OnUpdate)
//            .each([&](Entity ent, const TestComponent &test_comp) {
//                gz_info("without matching dirty {}", test_comp.test);
//        });
        
//        world.system("test_update")
//            .kind(flecs::OnUpdate)
//            .run([&](WorldIter& it) {
//            m_accumulated += it.delta_time();
//            while (m_accumulated > m_simulation_step_time) {
//                // Step the world
//                m_physics_system.Update(m_simulation_step_time, m_collision_step_per_simulate_step, m_temp_allocator, m_job_system);
//                m_accumulated -= m_simulation_step_time;
//            }
//        });

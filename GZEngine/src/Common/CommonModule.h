#pragma once
#include "Module.h"
#include "Log.h"
#include "ComponentInterface.h"

namespace GZ {
    // Specifically editor stuff
    struct EditorTrait{};

    struct DirtyTrait{};
    struct LoadingTrait{};
    struct ReadyTrait{};
	// declaration order does not matter

}

namespace GZ {
	// Expose this for each ecs module
	// register components, systems, triggers, hooks all here pre ticking the game
	struct CommonModule final : Module {
		void install_into(const ModuleContext& module_ctx) override;
		void uninstall_from(const ModuleContext& module_ctx) override;
	};
}

// ------------------------- Sync point examples --------


	//struct TestComponent {
	//	std::string test = "YoYo";
	//};
	//world.component<TestComponent>().add(flecs::Exclusive);

	//auto test_ent = world.entity().set<TestComponent, DirtyTrait>({ "ssdasdqw" });
	

	// Remove dirty trait is very important!!!
	//world.system<const TestComponent>("test_dirty")
	//.term_at(0).second<DirtyTrait>()
	//.write<TestComponent>()
	//.kind(flecs::OnUpdate)
	//.each([&](const Entity& ent, const TestComponent& test_comp) {
	//	ent.set<TestComponent>({ "Ready to roll!" });
	//	ent.set<vec3>({});
	//	ent.remove<TestComponent, DirtyTrait>();
	//	gz_info("With dirty matching {}", test_comp.test);
	//});

	//System physics_update = world.system<const TestComponent>("test_update")
	//.kind(flecs::OnUpdate)
	//.run([=](WorldIter& it) {
	//	gz_info("Hello test");
	//	if (test_ent.has<TestComponent, ReadyTrait>()) {
	//		gz_info("Has readytrait");
	//	}
	//	else {
	//		gz_info("Does not have readytrait");
	//	}

	//	if (test_ent.has<vec3>()) {
	//		gz_info("has vec3");
	//	}
	//});

	//world.system<const TestComponent>("test")
	//.term_at(0).second<ReadyTrait>()
	//.multi_threaded()
	//.kind(flecs::OnUpdate)
	//.each([&](Entity ent, const TestComponent& test_comp) {
	//	gz_info("without matching dirty {}", test_comp.test);
	//});
        
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

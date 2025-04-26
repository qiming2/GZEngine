#include <gzpch.h>

#include "TimerModule.h"
#include "MathUtil.h"

namespace GZ {

	void TimerModule::install_into(const ModuleContext& module_ctx)
	{
		GZ_TIMER_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);

		World &world = *module_ctx.world;

		System iterate_timers = world.system<TimerComponent>("iterate_timers")
			.kind(flecs::PreUpdate)
			.each([&](WorldIter& it, size_t index, TimerComponent &timer_comp) {
			if (is_approximately_zero(timer_comp.wait_time_in_secs)) {
				return;
			}
			
			timer_comp.elapsed_time += it.delta_time();
			if (timer_comp.wait_time_in_secs <= timer_comp.elapsed_time) {
				timer_comp.callback();

				if (!timer_comp.is_one_shot) {
					timer_comp.elapsed_time = timer_comp.elapsed_time - timer_comp.wait_time_in_secs;
				}
				else {
					timer_comp.wait_time_in_secs = 0.0f;
				}
			}
		});


	}

	void TimerModule::uninstall_from(const ModuleContext& module_ctx)
	{

	}

}
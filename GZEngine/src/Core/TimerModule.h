#pragma once
#include "Module.h"
#include "ComponentInterface.h"

#define GZ_TIMER_COMPONENT_VARS(GZ_COMPONENT_TYPE_DO, GZ_COMPONENT_MEMBER_TYPE_DO, GZ_COMPONENT_TYPE_END) \
		GZ_COMPONENT_TYPE_DO(TimerComponent) \
			GZ_COMPONENT_MEMBER_TYPE_DO(f32, wait_time_in_secs) \
			GZ_COMPONENT_MEMBER_TYPE_DO(f32, elapsed_time) \
			GZ_COMPONENT_MEMBER_TYPE_DO(b8, is_one_shot) \
			GZ_COMPONENT_MEMBER_TYPE_DO(std::function<void()>, callback) \
		GZ_COMPONENT_TYPE_END(TimerComponent) \

namespace GZ {
    
    struct TimerComponent {
        f32 wait_time_in_secs = 0.0f;
        f32 elapsed_time = 0.0f;
        b8 is_one_shot = true;
        std::function<void()> callback;
    };

    struct TimerModule final : Module {
        void install_into(const ModuleContext& module_ctx) override;
        void uninstall_from(const ModuleContext& module_ctx) override;

    private:

    };
}

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
	
	// declaration order does not matter

	// Many vec types already declared and defined, but not registered in world!
	// vec3 is already defined, we don't need to do these again
	
	GZ_TRANSFORM_COMPONENT_VARS(GZ_COMPONENT_TYPE_DECLARE, GZ_COMPONENT_MEMBER_TYPE_DECLARE, GZ_COMPONENT_TYPE_END_DECLARE);

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
	};
}



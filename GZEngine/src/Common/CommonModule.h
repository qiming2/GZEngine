#pragma once
#include "Module.h"
#include "Log.h"

// WARNINNNNNNNNNNNNNNNNNNNNNNG! Read through the end
// Some X macro for declare and register basic new component type
// This is useful for basic types like vec3, f32, primitive types but for string, vector
// we need to handle it a bit differently, maybe can still use macro but we'll see

// Basic Component declaration macro

// First macro declare component name
// Second macro declare component member type + member name
// Third macro ends the declaration scope, you should end with a ;
#define GZ_COMPONENT_TYPE_DECLARE(GZ_COMPONENT) \
	struct GZ_COMPONENT {
#define GZ_COMPONENT_MEMBER_TYPE_DECLARE(GZ_COMPONENT_MEMBER_TYPE, GZ_COMPONENT_MEMBER_NAME) \
	GZ_COMPONENT_MEMBER_TYPE GZ_COMPONENT_MEMBER_NAME;
#define GZ_COMPONENT_TYPE_END_DECLARE(GZ_COMPONENT) \
	}


// Basic Component definition macro

// First macro register component type to ecs/flecs world
// Assume the arg name of flecs world is always world!
// second macro register member info to the component type
// third macro ends component definition scope, you should end with a ;

#define GZ_COMPONENT_TYPE_DEFINE(GZ_COMPONENT) \
	world.component<GZ_COMPONENT>()
#define GZ_COMPONENT_TYPE_MEMBER_DEFINE(GZ_COMPONENT_MEMBER_TYPE, GZ_COMPONENT_MEMBER_NAME) \
	.member<GZ_COMPONENT_MEMBER_TYPE>(#GZ_COMPONENT_MEMBER_NAME)
#define GZ_COMPONENT_TYPE_END_DEFINE(GZ_COMPONENT)


////////////////////// Some examples of common components //////////////////////////////

// Declaration order does not matter but Registratin order in module install_into does matter like how c works

// glm registrations
#define GZ_VEC4_COMPONENT_VARS(GZ_COMPONENT_TYPE_DO, GZ_COMPONENT_MEMBER_TYPE_DO, GZ_COMPONENT_TYPE_END) \
	GZ_COMPONENT_TYPE_DO(vec4) \
		GZ_COMPONENT_MEMBER_TYPE_DO(f32, x) \
		GZ_COMPONENT_MEMBER_TYPE_DO(f32, y) \
		GZ_COMPONENT_MEMBER_TYPE_DO(f32, z) \
		GZ_COMPONENT_MEMBER_TYPE_DO(f32, w) \
	GZ_COMPONENT_TYPE_END(vec4) \

#define GZ_VEC3_COMPONENT_VARS(GZ_COMPONENT_TYPE_DO, GZ_COMPONENT_MEMBER_TYPE_DO, GZ_COMPONENT_TYPE_END) \
	GZ_COMPONENT_TYPE_DO(vec3) \
		GZ_COMPONENT_MEMBER_TYPE_DO(f32, x) \
		GZ_COMPONENT_MEMBER_TYPE_DO(f32, y) \
		GZ_COMPONENT_MEMBER_TYPE_DO(f32, z) \
	GZ_COMPONENT_TYPE_END(vec3) \

#define GZ_VEC2_COMPONENT_VARS(GZ_COMPONENT_TYPE_DO, GZ_COMPONENT_MEMBER_TYPE_DO, GZ_COMPONENT_TYPE_END) \
	GZ_COMPONENT_TYPE_DO(vec2) \
		GZ_COMPONENT_MEMBER_TYPE_DO(f32, x) \
		GZ_COMPONENT_MEMBER_TYPE_DO(f32, y) \
	GZ_COMPONENT_TYPE_END(vec2) \

#define GZ_TRANSFORM_COMPONENT_VARS(GZ_COMPONENT_TYPE_DO, GZ_COMPONENT_MEMBER_TYPE_DO, GZ_COMPONENT_TYPE_END) \
	GZ_COMPONENT_TYPE_DO(TransformComponent) \
		GZ_COMPONENT_MEMBER_TYPE_DO(vec3, position) \
		GZ_COMPONENT_MEMBER_TYPE_DO(vec3, scale) \
		GZ_COMPONENT_MEMBER_TYPE_DO(vec3, rotation) \
	GZ_COMPONENT_TYPE_END(TransformComponent) \

namespace GZ {
	
	// declaration order does not matter

	// Many vec types already declared and defined, but not registered in world!
	// vec3 is already defined, we don't need to do these again
	
	GZ_TRANSFORM_COMPONENT_VARS(GZ_COMPONENT_TYPE_DECLARE, GZ_COMPONENT_MEMBER_TYPE_DECLARE, GZ_COMPONENT_TYPE_END_DECLARE);
	
	extern void draw_component_imgui_ui_transform(void *component);

	/*template<>
	void*/ 
}

// Component formatter
template<>
struct fmt::formatter<GZ::TransformComponent> : fmt::formatter<std::string>
{
	auto format(GZ::TransformComponent my_transform, format_context& ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "\np: {}\ns: {}\nr: {}", my_transform.position, my_transform.scale, my_transform.rotation);
	}
};

namespace GZ {
	// Expose this for each ecs module
	// register components, systems, triggers, hooks all here pre ticking the game
	struct CommonModule final : Module {
		void install_into(World &world) override;
		void uninstall_from(World &world) override;
	};
}



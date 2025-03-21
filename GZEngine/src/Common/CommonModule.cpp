#include <gzpch.h>
#include "CommonModule.h"

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

#define GZ_b8_COMPONENT_VARS(GZ_COMPONENT_TYPE_DO, GZ_COMPONENT_MEMBER_TYPE_DO, GZ_COMPONENT_TYPE_END) \
	GZ_COMPONENT_TYPE_DO(b8) \
	GZ_COMPONENT_TYPE_END(b8) \

namespace GZ {
	
	// Primitive draw component
	struct DrawComponentImplStructName(vec4) final : IDrawComponentInterfaceName 
	{ 
		void draw_imgui(void* comp, const ComponentRegistry *registry, World *world, DrawComponentContext *draw_ctx) override {
			vec4 *vec_comp = static_cast<vec4 *>(comp);
			const char * label = draw_ctx ? draw_ctx->name.data() : LOCATION;
			ImGui::DragFloat4(label, &vec_comp->x);
		}
	};

	struct DrawComponentImplStructName(vec3) final : IDrawComponentInterfaceName
	{
		void draw_imgui(void* comp, const ComponentRegistry * registry, World * world, DrawComponentContext * draw_ctx) override {
			vec3* vec_comp = static_cast<vec3*>(comp);
			const char* label = draw_ctx && draw_ctx->name.data() ? draw_ctx->name.data() : LOCATION;
			ImGui::DragFloat3(label, &vec_comp->x);
		}
	};

	struct DrawComponentImplStructName(vec2) final : IDrawComponentInterfaceName
	{
		void draw_imgui(void* comp, const ComponentRegistry * registry, World * world, DrawComponentContext * draw_ctx) override {
			vec2* vec_comp = static_cast<vec2*>(comp);
			const char* label = draw_ctx ? draw_ctx->name.data() : LOCATION;
			ImGui::DragFloat2(label, &vec_comp->x);
		}
	};

	struct DrawComponentImplStructName(b8) final : IDrawComponentInterfaceName
	{
		void draw_imgui(void* comp, const ComponentRegistry* registry, World* world, DrawComponentContext* draw_ctx) override {
			b8* b8_comp = static_cast<b8*>(comp);
			const char* label = draw_ctx ? draw_ctx->name.data() : LOCATION;
			ImGui::Checkbox("label", b8_comp);
		}
	};

	GZ_TRANSFORM_COMPONENT_VARS(GZ_COMPONENT_TYPE_IMPL_DRAW, GZ_COMPONENT_MEMBER_TYPE_IMPL_DRAW, GZ_COMPONENT_TYPE_END_IMPL_DRAW);

	void CommonModule::install_into(World& world, ComponentRegistry &reg)
	{
		
		// we register primitive types first otherwise
		GZ_b8_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);
		
		GZ_b8_COMPONENT_VARS(GZ_COMPONENT_TYPE_IMPL_DRAW_REG, GZ_COMPONENT_MEMBER_TYPE_IMPL_DRAW_REG, GZ_COMPONENT_TYPE_END_IMPL_DRAW_REG);


		// flecs does not know how to register transform component which
		// contains vec3, it works just like c/c++
		
		GZ_VEC4_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);

		GZ_VEC4_COMPONENT_VARS(GZ_COMPONENT_TYPE_IMPL_DRAW_REG, GZ_COMPONENT_MEMBER_TYPE_IMPL_DRAW_REG, GZ_COMPONENT_TYPE_END_IMPL_DRAW_REG);

		GZ_VEC3_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);

		GZ_VEC3_COMPONENT_VARS(GZ_COMPONENT_TYPE_IMPL_DRAW_REG, GZ_COMPONENT_MEMBER_TYPE_IMPL_DRAW_REG, GZ_COMPONENT_TYPE_END_IMPL_DRAW_REG);

		GZ_VEC2_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);

		GZ_VEC2_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);
		// Complex component

		GZ_TRANSFORM_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);

		GZ_TRANSFORM_COMPONENT_VARS(GZ_COMPONENT_TYPE_IMPL_DRAW_REG, GZ_COMPONENT_MEMBER_TYPE_IMPL_DRAW_REG, GZ_COMPONENT_TYPE_END_IMPL_DRAW_REG);

		//// Need to register transform
		//// This is what the above macro would expand to
		//ComponentID vec3_id = world.component<vec3>().id();
		//reg.draw_component_interfaces[vec3_id] = std::make_shared<DrawComponentImplStructName(vec3)>();
		/*
		auto transform_comp = world.component<TransformComponent>()
			.member<vec3>("position")
			.member<vec3>("scale");
			.member<vec3>("rotation); 
		*/

	}

	void CommonModule::uninstall_from(World& world, ComponentRegistry &registry)
	{
		// don't anything for now
	}

	
}




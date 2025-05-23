#include <gzpch.h>
#include "CommonModule.h"

#include "Profiler.h"
// glm registrations
#define GZ_VEC4_COMPONENT_VARS(GZ_COMPONENT_TYPE_DO, GZ_COMPONENT_MEMBER_TYPE_DO, GZ_COMPONENT_TYPE_END) \
	GZ_COMPONENT_TYPE_DO(vec4) \
		GZ_COMPONENT_MEMBER_TYPE_DO(f32, x) \
		GZ_COMPONENT_MEMBER_TYPE_DO(f32, y) \
		GZ_COMPONENT_MEMBER_TYPE_DO(f32, z) \
		GZ_COMPONENT_MEMBER_TYPE_DO(f32, w) \
	GZ_COMPONENT_TYPE_END(vec4) \

#define GZ_QUAT_COMPONENT_VARS(GZ_COMPONENT_TYPE_DO, GZ_COMPONENT_MEMBER_TYPE_DO, GZ_COMPONENT_TYPE_END) \
    GZ_COMPONENT_TYPE_DO(quat) \
        GZ_COMPONENT_MEMBER_TYPE_DO(f32, x) \
        GZ_COMPONENT_MEMBER_TYPE_DO(f32, y) \
        GZ_COMPONENT_MEMBER_TYPE_DO(f32, z) \
        GZ_COMPONENT_MEMBER_TYPE_DO(f32, w) \
    GZ_COMPONENT_TYPE_END(quat) \

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

#define GZ_f32_COMPONENT_VARS(GZ_COMPONENT_TYPE_DO, GZ_COMPONENT_MEMBER_TYPE_DO, GZ_COMPONENT_TYPE_END) \
    GZ_COMPONENT_TYPE_DO(f32) \
    GZ_COMPONENT_TYPE_END(f32) \

namespace GZ {
	
	// Primitive draw component
	struct DrawComponentImplStructName(vec4) final : IDrawComponentInterfaceName 
	{ 
		void draw_imgui(void* comp, const ModuleContext *module_ctx, DrawComponentContext *draw_ctx) override {
			vec4 *my_comp = static_cast<vec4 *>(comp);
			const char * label = draw_ctx ? draw_ctx->name.data() : LOCATION;
			ImGui::DragFloat4(label, &my_comp->x);
		}
	};

    // Primitive draw component
    struct DrawComponentImplStructName(quat) final : IDrawComponentInterfaceName
    {
		// This is weird
        void draw_imgui(void* comp, const ModuleContext *module_ctx, DrawComponentContext *draw_ctx) override {
			
			{
                quat* my_comp = static_cast<quat*>(comp);
				vec3 origin_angles = glm::degrees(glm::eulerAngles(*my_comp));
				//normalize_angles(origin_angles);
				
				vec3 euler_angles = origin_angles;
				

				const char* label = draw_ctx ? draw_ctx->name.data() : LOCATION;
				ImGui::DragFloat3(label, glm::value_ptr(euler_angles));
				//ImGui::DragFloat4(label, glm::value_ptr(*my_comp));
				//glm::quat(1)
				glm::vec3 delta = glm::radians(euler_angles - origin_angles);

				*my_comp = glm::normalize(*my_comp * glm::quat(delta));
				//*my_comp = glm::quat(glm::radians(euler_angles));
			}
        }
    };

	struct DrawComponentImplStructName(vec3) final : IDrawComponentInterfaceName
	{
		void draw_imgui(void* comp, const ModuleContext *module_ctx, DrawComponentContext * draw_ctx) override {
			vec3* my_comp = static_cast<vec3*>(comp);
			const char* label = draw_ctx && draw_ctx->name.data() ? draw_ctx->name.data() : LOCATION;
			ImGui::DragFloat3(label, &my_comp->x);
		}
	};

	struct DrawComponentImplStructName(vec2) final : IDrawComponentInterfaceName
	{
		void draw_imgui(void* comp, const ModuleContext *module_ctx, DrawComponentContext * draw_ctx) override {
			vec2* my_comp = static_cast<vec2*>(comp);
			const char* label = draw_ctx ? draw_ctx->name.data() : LOCATION;
			ImGui::DragFloat2(label, &my_comp->x);
		}
	};

	struct DrawComponentImplStructName(b8) final : IDrawComponentInterfaceName
	{
		void draw_imgui(void* comp, const ModuleContext *module_ctx, DrawComponentContext* draw_ctx) override {
			b8* b8_comp = static_cast<b8*>(comp);
			const char* label = draw_ctx ? draw_ctx->name.data() : LOCATION;
			ImGui::Checkbox(label, b8_comp);
		}
	};

    struct DrawComponentImplStructName(f32) final : IDrawComponentInterfaceName
    {
        void draw_imgui(void* comp, const ModuleContext *module_ctx, DrawComponentContext* draw_ctx) override {
            f32* f32_comp = static_cast<f32*>(comp);
            const char* label = draw_ctx ? draw_ctx->name.data() : LOCATION;
            ImGui::DragFloat(label, f32_comp);
        }
    };

	void CommonModule::install_into(const ModuleContext &module_ctx)
	{
		World &world = *module_ctx.world;
		ComponentRegistry &reg = *module_ctx.reg;
		// we register primitive types first otherwise
		GZ_b8_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);
		
		GZ_b8_COMPONENT_VARS(GZ_COMPONENT_TYPE_IMPL_DRAW_REG, GZ_COMPONENT_MEMBER_TYPE_IMPL_DRAW_REG, GZ_COMPONENT_TYPE_END_IMPL_DRAW_REG);


        GZ_f32_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);
        
        GZ_f32_COMPONENT_VARS(GZ_COMPONENT_TYPE_IMPL_DRAW_REG, GZ_COMPONENT_MEMBER_TYPE_IMPL_DRAW_REG, GZ_COMPONENT_TYPE_END_IMPL_DRAW_REG);
		// flecs does not know how to register transform component which
		// contains vec3, it works just like c/c++
		
		GZ_VEC4_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);

		GZ_VEC4_COMPONENT_VARS(GZ_COMPONENT_TYPE_IMPL_DRAW_REG, GZ_COMPONENT_MEMBER_TYPE_IMPL_DRAW_REG, GZ_COMPONENT_TYPE_END_IMPL_DRAW_REG);
        
        GZ_QUAT_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);

        GZ_QUAT_COMPONENT_VARS(GZ_COMPONENT_TYPE_IMPL_DRAW_REG, GZ_COMPONENT_MEMBER_TYPE_IMPL_DRAW_REG, GZ_COMPONENT_TYPE_END_IMPL_DRAW_REG);

		GZ_VEC3_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);

		GZ_VEC3_COMPONENT_VARS(GZ_COMPONENT_TYPE_IMPL_DRAW_REG, GZ_COMPONENT_MEMBER_TYPE_IMPL_DRAW_REG, GZ_COMPONENT_TYPE_END_IMPL_DRAW_REG);

		GZ_VEC2_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);

		GZ_VEC2_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);
		
        world.component<EditorTrait>();
        world.component<DirtyTrait>();
        world.component<LoadingTrait>();
        world.component<ReadyTrait>();

	}

	void CommonModule::uninstall_from(const ModuleContext &module_ctx)
	{
		// don't anything for now
	}

}




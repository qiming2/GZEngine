#pragma once
#include <defines.h>
#include <imgui.h>
#include "Log.h"

// WARNINNNNNNNNNNNNNNNNNNNNNNG! Read through the end
// Some X macro for declare and register basic new component type
// This is useful for basic types like vec3, f32, primitive types but for string, vector
// we need to handle it a bit differently, maybe can still use macro but we'll see

// All these can be manually typed out for customization
// Here are some macros for quick and dirty way to iterate things fast

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
	module_ctx.world->component<GZ_COMPONENT>()
#define GZ_COMPONENT_TYPE_MEMBER_DEFINE(GZ_COMPONENT_MEMBER_TYPE, GZ_COMPONENT_MEMBER_NAME) \
	.member<GZ_COMPONENT_MEMBER_TYPE>(#GZ_COMPONENT_MEMBER_NAME)
#define GZ_COMPONENT_TYPE_END_DEFINE(GZ_COMPONENT)


// Some no ops
#define GZ_COMPONENT_TYPE_MEMBER_NOP(GZ_COMPONENT_MEMBER_TYPE, GZ_COMPONENT_MEMBER_NAME)
#define GZ_COMPONENT_TYPE_END_NOP(GZ_COMPONENT) 

// DrawComponentInterface
#define IDrawComponentInterfaceName _IDrawComponentInterfaceName
#define _IDrawComponentInterfaceName IDrawComponentInterface


// Basic Draw
namespace GZ {
	struct DrawComponentContext {
		std::string_view name;
		Entity e;
	};

	struct ComponentRegistry;
	struct ModuleContext;

	struct IDrawComponentInterfaceName {
		virtual ~IDrawComponentInterfaceName() {}
		virtual void draw_imgui(void *comp, const ModuleContext *module_ctx, DrawComponentContext *draw_ctx) {
			gz_warn("This component does not have a draw function");
		}
	};

}

// Convenience macro for draw

// DrawComponentImpl base name and component impl struct name
#define DrawComponentImplName DrawComponentImpl
#define DrawComponentImplStructName(GZ_COMPONENT) GZ_CAT(DrawComponentImplName, GZ_COMPONENT)

// DrawComponentImpl definition
#define GZ_COMPONENT_TYPE_IMPL_DRAW(GZ_COMPONENT) \
	struct DrawComponentImplStructName(GZ_COMPONENT) final : IDrawComponentInterfaceName { \
		void draw_imgui(void *comp, const ModuleContext *module_ctx, DrawComponentContext *draw_ctx) override { \
			ComponentID comp_id = module_ctx->world->component<GZ_COMPONENT>().id();\
			ImGui::PushID(comp_id);\
			static const char *comp_name = #GZ_COMPONENT;\
			GZ_COMPONENT *actual_comp = static_cast<GZ_COMPONENT *>(comp); \
			
#define GZ_COMPONENT_MEMBER_TYPE_IMPL_DRAW(GZ_COMPONENT_MEMBER_TYPE, GZ_COMPONENT_MEMBER_NAME) \
			do { \
				ComponentID GZ_CAT(GZ_COMPONENT_MEMBER_NAME, _comp_id) = module_ctx->world->component<GZ_COMPONENT_MEMBER_TYPE>().id(); \
				std::shared_ptr<IDrawComponentInterfaceName> GZ_CAT(GZ_COMPONENT_MEMBER_NAME, _comp_interface) = module_ctx->reg->get_draw_interface(GZ_CAT(GZ_COMPONENT_MEMBER_NAME, _comp_id)); \
				if (GZ_CAT(GZ_COMPONENT_MEMBER_NAME, _comp_interface) != nullptr) {\
					DrawComponentContext cur_draw_ctx;\
					cur_draw_ctx.name = #GZ_COMPONENT_MEMBER_NAME;\
                    GZ_CAT(GZ_COMPONENT_MEMBER_NAME, _comp_interface)->draw_imgui(&actual_comp->GZ_COMPONENT_MEMBER_NAME, module_ctx, &cur_draw_ctx); \
				} else { \
					gz_warn("{} does not implement {} interface", #GZ_COMPONENT_MEMBER_TYPE, GZ_STR(IDrawComponentInterfaceName)); \
				}\
			} while (0);
				
#define GZ_COMPONENT_TYPE_END_IMPL_DRAW(GZ_COMPONENT) \
			if (draw_ctx->e) {\
				draw_ctx->e.modified(comp_id);\
			}\
			ImGui::PopID();\
		} \
	}

// Register DrawComponentImpl definition
#define GZ_COMPONENT_TYPE_IMPL_DRAW_REG(GZ_COMPONENT) \
	do {\
		ComponentID GZ_CAT(GZ_COMPONENT, _id) = module_ctx.world->component<GZ_COMPONENT>().id(); \
		module_ctx.reg->draw_component_interfaces[GZ_CAT(GZ_COMPONENT, _id)] = \
			std::make_shared<DrawComponentImplStructName(GZ_COMPONENT)>(); \
	} while(0)
#define GZ_COMPONENT_MEMBER_TYPE_IMPL_DRAW_REG(GZ_COMPONENT_MEMBER_TYPE, GZ_COMPONENT_MEMBER_NAME) GZ_COMPONENT_TYPE_MEMBER_NOP(GZ_COMPONENT_MEMBER_TYPE, GZ_COMPONENT_MEMBER_NAME)

#define GZ_COMPONENT_TYPE_END_IMPL_DRAW_REG(GZ_COMPONENT) GZ_COMPONENT_TYPE_END_NOP(GZ_COMPONENT)

// Component Registry
namespace GZ {
	struct ComponentRegistry {

		inline std::shared_ptr<IDrawComponentInterfaceName> get_draw_interface(ComponentID comp_id) const {
			auto it = draw_component_interfaces.find(comp_id);
			if (it != draw_component_interfaces.end()) {
				return it->second;
			}
			return nullptr;
		}

		std::unordered_map<ComponentID, std::shared_ptr<IDrawComponentInterfaceName>> draw_component_interfaces;
	};
}

#include <gzpch.h>
#include <CommonModule.h>

namespace GZ {

	void draw_component_imgui_ui_transform(void* component) {
		TransformComponent& transform = *static_cast<TransformComponent*>(component);
		ImGui::DragFloat3("Position", &transform.position[0]);
	}

	void CommonModule::install_into(World& world)
	{
		
		// we register math types first otherwise
		// flecs does not know how to register transform component which
		// contains vec3, it works just like c/c++
		
		GZ_VEC4_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);

		GZ_VEC2_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);

		GZ_VEC3_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);

		GZ_TRANSFORM_COMPONENT_VARS(GZ_COMPONENT_TYPE_DEFINE, GZ_COMPONENT_TYPE_MEMBER_DEFINE, GZ_COMPONENT_TYPE_END_DEFINE);
		
		// This is what the above macro would expand to
		
		/*
		auto transform_comp = world.component<TransformComponent>()
			.member<vec3>("position")
			.member<vec3>("scale");
			.member<vec3>("rotation); 
		*/

	}

	void CommonModule::uninstall_from(World& world)
	{
		// don't anything for now
	}

	
}




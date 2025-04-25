#include <gzpch.h>

#include "ComponentInterface.h"
#include "TransformModule.h"

namespace GZ {

	GZ_TRANSFORM_COMPONENT_VARS(GZ_COMPONENT_TYPE_IMPL_DRAW, GZ_COMPONENT_MEMBER_TYPE_IMPL_DRAW, GZ_COMPONENT_TYPE_END_IMPL_DRAW);

	void TransformModule::install_into(const ModuleContext& module_ctx)
	{
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

	void TransformModule::uninstall_from(const ModuleContext& module_ctx)
	{

	}

	mat4 TransformModule::world_transform(const Entity &entity)
	{
		return mat4(1.0);
	}

	mat4 TransformModule::world_transform(const EntityID entity)
	{
		return mat4(1.0);
	}

	mat4 TransformModule::local_transform(const Entity &entity)
	{
		return entity.get<TransformComponent>()->get_matrix();
	}

	mat4 TransformModule::local_transform(const EntityID entity)
	{
		return m_world->id(entity).entity().get<TransformComponent>()->get_matrix();
	}

	void TransformModule::update_world_transform()
	{

	}

}
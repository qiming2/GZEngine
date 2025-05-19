#include <gzpch.h>

#include "ComponentInterface.h"
#include "TransformModule.h"


namespace GZ {

	GZ_TRANSFORM_COMPONENT_VARS(GZ_COMPONENT_TYPE_IMPL_DRAW, GZ_COMPONENT_MEMBER_TYPE_IMPL_DRAW, GZ_COMPONENT_TYPE_END_IMPL_DRAW);

	void TransformModule::install_into(const ModuleContext& module_ctx)
	{
		// Complex component
		m_world = module_ctx.world;
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
		if (m_reparent_ob.is_valid())
			m_reparent_ob.destruct();
		if (m_on_set_ob.is_valid())
			m_on_set_ob.destruct();
	}

	void TransformModule::end_install(const ModuleContext& module_ctx)
	{
		m_scene_module = module_ctx.module_reg->get_module<SceneModule>();
		m_scene_root = m_scene_module->get_scene_root_entity();
		m_world_transform_cache[m_scene_root.id()] = GZ_MAT4_IDENTITY;

		// Reparenting should trigger dirty
		m_reparent_ob = m_world->observer<TransformComponent>("Reparenting")
			.term_at(0).up()
			.event(flecs::OnAdd)
			.event(flecs::OnRemove)
			.each([&](flecs::iter& it, size_t i, const TransformComponent&) {
			const Entity& e = it.world().id(it.entities()[i]).entity();

			// At the time of reparenting, it does not need to check whether
			// this entity has transform component
			if (e != m_scene_root)
				private_mark_entity_local_transform_dirty_no_check(e);
		});

		// On set should trigger dirty
		m_on_set_ob = m_world->observer<TransformComponent>("On set")
			.event(flecs::OnSet)
			.each([&](flecs::iter& it, size_t i, const TransformComponent&) {
			const Entity& e = it.world().id(it.entities()[i]).entity();
			if (e != m_scene_root)
				mark_entity_local_transform_dirty(e);
		});
	}

	mat4 TransformModule::world_transform(const Entity& entity)
	{
		gz_assert(entity.is_valid(), "Entity is not valid");
		//gz_info("Entity id: {}", entity.id());
		if (entity == m_scene_root) return GZ_MAT4_IDENTITY;
		
		auto it = m_world_transform_cache.find(entity.id());
		if (it != m_world_transform_cache.end()) {
			return it->second;
		}
		
		Entity parent = entity.parent();
		mat4 entity_world_transform = world_transform(parent) * local_transform(entity);
		m_world_transform_cache[entity.id()] = entity_world_transform;
		TransformComponent t_comp;
		t_comp.from_mat4(entity_world_transform);
		m_world_transform_component_cache[entity.id()] = t_comp;

		return entity_world_transform;
	}

	mat4 TransformModule::world_transform(const EntityID entity_id)
	{
		
		return world_transform(m_world->id(entity_id).entity());
	}

	TransformComponent TransformModule::world_transform_component(const Entity& entity)
	{
		world_transform(entity);
		return m_world_transform_component_cache[entity];
	}

	TransformComponent TransformModule::world_transform_component(const EntityID entity_id)
	{

		return world_transform_component(m_world->id(entity_id).entity());
	}

	GZ_FORCE_INLINE mat4 TransformModule::local_transform(const Entity &entity) const
	{
		gz_assert(entity.has<TransformComponent>(), "Entity does not have transform component!");
		return entity.get<TransformComponent>()->get_mat4();
	}

	GZ_FORCE_INLINE mat4 TransformModule::local_transform(const EntityID entity_id) const
	{
		Entity entity = m_world->id(entity_id).entity();
		gz_assert(entity.has<TransformComponent>(), "Entity does not have transform component!");
		return entity.get<TransformComponent>()->get_mat4();
	}

	void TransformModule::mark_entity_local_transform_dirty(const Entity &entity)
	{
		gz_assert(entity.has<TransformComponent>(), "Entity does not have transform component!");
		// invalidate all the caches in its children as well
		private_mark_entity_local_transform_dirty_no_check(entity);
	}

	void TransformModule::mark_entity_local_transform_dirty(EntityID entity_id)
	{
		Entity entity = m_world->id(entity_id).entity();
		gz_assert(entity.has<TransformComponent>(), "Entity does not have transform component!");
		private_mark_entity_local_transform_dirty_no_check(entity);
	}

	void TransformModule::private_mark_entity_local_transform_dirty_no_check(const Entity &entity)
	{
		private_mark_cache_dirty(entity);

		entity.children([&](Entity child) {
			private_mark_entity_local_transform_dirty_no_check(child);
		});
	}

	void TransformModule::private_mark_cache_dirty(const Entity& entity) {
		auto it = m_world_transform_cache.find(entity.id());
		if (it == m_world_transform_cache.end()) {
			return;
		}
		else {
			m_world_transform_cache.erase(it);
		}

		auto it_comp = m_world_transform_component_cache.find(entity.id());
		if (it_comp == m_world_transform_component_cache.end()) {
			return;
		}
		else {
			m_world_transform_component_cache.erase(it_comp);
		}
		
	}

}
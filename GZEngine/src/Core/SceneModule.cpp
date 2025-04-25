#include <gzpch.h>

#include "SceneModule.h"
#include "TransformModule.h"
#include "MathUtil.h"

namespace GZ {
	void SceneModule::install_into(const ModuleContext& module_ctx)
	{

		// Scene root singleton
		m_world = module_ctx.world;
		m_scene_root = m_world->entity("SceneRoot")
			.set<TransformComponent>({GZ_TRANSLATION_ZERO, GZ_QUAT_IDENTITY, GZ_SCALE_ZERO});
	}
	
	void SceneModule::uninstall_from(const ModuleContext& module_ctx)
	{

	}

	Entity SceneModule::lookup(const char *name)
	{
		return m_scene_root.lookup(name);
	}

	Entity SceneModule::entity(const char *name)
	{
		return m_world->entity(name).child_of(m_scene_root);
	}

	Entity SceneModule::get_scene_root_entity() const
	{
		return m_scene_root;
	}

	EntityID SceneModule::get_scene_root_id() const
	{
		return m_scene_root.id();
	}

}
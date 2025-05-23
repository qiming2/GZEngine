#pragma once
#include "Module.h"
#include "Log.h"
#include "ComponentInterface.h"
#include "SceneModule.h"
#include "MathUtil.h"

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
	// Many vec types already declared and defined, but not registered in world!
	// vec3 is already defined, we don't need to do these again


	//	GZ_TRANSFORM_COMPONENT_VARS(GZ_COMPONENT_TYPE_DECLARE, GZ_COMPONENT_MEMBER_TYPE_DECLARE, GZ_COMPONENT_TYPE_END_DECLARE);

	struct TransformComponent {
		vec3 p = GZ_TRANSLATION_ZERO;
		quat r = GZ_QUAT_IDENTITY;
		vec3 s = GZ_SCALE_ONE;

		GZ_FORCE_INLINE mat4 get_mat4() const {
			return glm::translate(mat4(1.0f), p) * glm::mat4_cast(r) * glm::scale(mat4(1.0f), s);
		}

		GZ_FORCE_INLINE void from_mat4(const mat4 &mat) {
			p = vec3(mat[3]);
			
			vec3 col0 = vec3(mat[0]);
			vec3 col1 = vec3(mat[1]);
			vec3 col2 = vec3(mat[2]);

			s.x = glm::length(col0);
			s.y = glm::length(col1);
			s.z = glm::length(col2);

			if (s.x != 0.0) col0 /= s.x;
			if (s.y != 0.0) col1/= s.y;
			if (s.z != 0.0) col2 /= s.z;

			mat3 r_mat = mat3(col0, col1, col2);
			r = glm::quat_cast(r_mat);
		}

	};

    struct TransformModule final : Module {
	public:
        void install_into(const ModuleContext& module_ctx) override;
        void uninstall_from(const ModuleContext& module_ctx) override;
		void end_install(const ModuleContext& module_ctx) override;
	public:
		GZ_API mat4 world_transform(const Entity &entity);
		GZ_API mat4 world_transform(EntityID entity_id);
        GZ_FORCE_INLINE GZ_API mat4 local_transform(const Entity &entity) const;
        GZ_FORCE_INLINE GZ_API mat4 local_transform(EntityID entity_id) const;
		GZ_API TransformComponent world_transform_component(const Entity& entity);
		GZ_API TransformComponent world_transform_component(EntityID entity_id);

		GZ_API void mark_entity_local_transform_dirty(const Entity& entity);
        GZ_API void mark_entity_local_transform_dirty(EntityID entity_id);
		GZ_API void clear_cache();
		
	private:
		void private_mark_cache_dirty(const Entity& entity);
		void private_mark_entity_local_transform_dirty_no_check(const Entity &entity);
		SceneModule *m_scene_module = nullptr;
		std::unordered_map<EntityID, mat4> m_world_transform_cache;
		std::unordered_map<EntityID, TransformComponent> m_world_transform_component_cache;
		World *m_world = nullptr;
		Entity m_scene_root;

		Observer m_reparent_ob;
		Observer m_on_set_ob;
    };
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

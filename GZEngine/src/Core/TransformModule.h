#pragma once
#include "Module.h"
#include "Log.h"
#include "ComponentInterface.h"
#include "SceneModule.h"

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
		vec3 p = vec3{ 0, 0, 0 };
		quat r = quat{1, 0, 0, 0};
		vec3 s = vec3{1, 1, 1};

		GZ_FORCE_INLINE mat4 get_matrix() const {
			return glm::translate(mat4(1.0f), p) * glm::mat4_cast(r) * glm::scale(mat4(1.0f), s);
		}
	};

    struct TransformModule final : Module {
        void install_into(const ModuleContext& module_ctx) override;
        void uninstall_from(const ModuleContext& module_ctx) override;
		mat4 world_transform(const Entity &entity);
		GZ_FORCE_INLINE mat4 local_transform(const Entity &entity);
		mat4 world_transform(EntityID entity);
		GZ_FORCE_INLINE mat4 local_transform(EntityID entity);
		void update_world_transform();
	private:
		SceneModule *m_scene_module = nullptr;
		std::unordered_map<EntityID, mat4> m_world_transform_caches;
		World *m_world = nullptr;
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

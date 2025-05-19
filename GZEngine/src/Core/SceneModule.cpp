#include <gzpch.h>

#include "SceneModule.h"
#include "TransformModule.h"
#include "MathUtil.h"

namespace GZ {
	void SceneModule::install_into(const ModuleContext& module_ctx)
	{
		// Scene root singleton
		m_world = module_ctx.world;
		// Register reflection for std::string
		m_world->component<std::string>()
			.opaque(flecs::String) // Opaque type that maps to string
			.serialize([](const flecs::serializer* s, const std::string* data) {
			const char* str = data->c_str();
			return s->value(flecs::String, &str); // Forward to serializer
		})
			.assign_string([](std::string* data, const char* value) {
			*data = value; // Assign new value to std::string
		});

		m_world->component<TagComponent>()
			.member<std::string>("name");
		
	}
	
	void SceneModule::uninstall_from(const ModuleContext& module_ctx)
	{
		clear_scene();
		m_scene_root.destruct();
	}

	void SceneModule::clear_scene() {
		m_world->delete_with(flecs::ChildOf, m_scene_root);
		if (m_cur_scene_prefab)
			m_cur_scene_prefab.destruct();
	}

	void SceneModule::after_install(const ModuleContext& module_ctx)
	{
		m_scene_root = m_world->entity<SceneRoot>();

	}

	void SceneModule::end_install(const ModuleContext& module_ctx)
	{
		create_new_scene();
	}

	Entity SceneModule::create_new_scene()
	{
		clear_scene();

		/*m_cur_scene_prefab = m_world->prefab("Scene")
			.set<TagComponent>({"New Scene"});
			Prefab new_one = m_world->prefab("1").child_of(m_cur_scene_prefab).set<TagComponent>({"New 1"}).slot_of(m_cur_scene_prefab);;
				Prefab new_three = m_world->prefab("3").child_of(new_one).set<TagComponent>({"New 3"}).slot_of(m_cur_scene_prefab);;
					Prefab new_four = m_world->prefab("4").child_of(new_three).set<TagComponent>({"New 4"}).slot_of(m_cur_scene_prefab);;
			Prefab new_two = m_world->prefab("2").child_of(m_cur_scene_prefab).set<TagComponent>({"New 2"}).slot_of(m_cur_scene_prefab);;
				Prefab new_five = m_world->prefab("5").child_of(new_two).set<TagComponent>({"New 5"}).slot_of(m_cur_scene_prefab);;*/
		//gz_info("Create Scene {}", new_three.path().c_str());

		const char* json = R"json(
    {
      "results": [
		{"parent":"GZ.SceneRoot", "name":"#754", "components":{"GZ.TransformComponent":{"p":{"x":0, "y":0, "z":0}, "r":{"x":0, "y":0, "z":0, "w":1}, "s":{"x":1, "y":1, "z":1}}, "GZ.TagComponent":{"name":"json_des"}}},
        {"parent":"#754", "name":"#888", "components":{"GZ.TransformComponent":{"p":{"x":1, "y":7.2881717682, "z":1}, "r":{"x":0, "y":0, "z":0, "w":1}, "s":{"x":1, "y":1, "z":1}}, "GZ.TagComponent":{"name":"json_des"}}},
        {"parent":"#754", "name":"#999", "components":{"GZ.TransformComponent":{"p":{"x":1, "y":7.2881717682, "z":1}, "r":{"x":0, "y":0, "z":0, "w":1}, "s":{"x":1, "y":1, "z":1}}, "GZ.TagComponent":{"name":"json_des2"}}}
      ]
    })json";

		/*m_cur_scene = m_world->entity().child_of(m_scene_root)
			.set<TagComponent>({ "New Scene" })
			.add<TransformComponent>();*/
		struct LoadContext {
			std::vector<Entity> pre_loaded_entities;
		};

		LoadContext ctx;
		ctx.pre_loaded_entities.emplace_back(m_world->entity());
		ctx.pre_loaded_entities.emplace_back(m_world->entity());
		ctx.pre_loaded_entities.emplace_back(m_world->entity());
		flecs::from_json_desc_t desc;
		desc.lookup_ctx = &ctx;
		//desc.strict = true;
		desc.name = "SceneLoadTest";

		desc.lookup_action = [](const WorldID* world_id, const char *value, void* ctx) -> EntityID {
			gz_info(value);
			World world((WorldID *)world_id);

			LoadContext *load_ctx = (LoadContext *)ctx;
			if (!strcmp(value, "#888")) {
				return load_ctx->pre_loaded_entities[1].id();
			}
			else if (!strcmp(value, "#999")) {
				return load_ctx->pre_loaded_entities[2].id();
			}
			else if (!strcmp(value, "#754")) {
				return load_ctx->pre_loaded_entities[0].id();
			}

			IdentifierID id = ecs_lookup(world_id, value);
			
			if (id) {
				return id;
			}
			
			gz_assert(false, "Should not reach here, Sceneload failed");
			return 0;
			
		};
		m_world->from_json(json, &desc);
		m_scene_root.children([&](Entity child) {
			m_cur_scene = child;
		});
		
		// Entity json test
		/*const char* json_test = R"json({"parent":"#754", "name":"#888", "components":{"GZ.TransformComponent":{"p":{"x":1, "y":7.2881717682, "z":1}, "r":{"x":0, "y":0, "z":0, "w":1}, "s":{"x":1, "y":1, "z":1}}, "GZ.TagComponent":{"name":"sphere"}}})json";
		Entity e = m_world->entity();
		if (!e.has<TagComponent>()) {
			gz_info("does not have tag component");
		}
		e.from_json(json_test);
		gz_info("{}", e.id());
		*/

		
		

		return m_cur_scene;
	}

	Prefab SceneModule::get_active_scene_prefab()
	{
		return m_cur_scene_prefab;
	}

	Entity SceneModule::get_active_scene()
	{
		return m_cur_scene;
	}

	Prefab SceneModule::prefab(Prefab parent)
	{
		if (!m_cur_scene_prefab) {
			gz_warn("Prefab does not exist!");
			return Prefab();
		}
		Prefab new_prefab = m_world->prefab()
			.set<TagComponent>({"New Entity"})
			.add<TransformComponent>()
			.child_of(parent);

		if (m_cur_scene)
			m_cur_scene.destruct();

		m_cur_scene = m_world->entity()
			.is_a(m_cur_scene_prefab)
			.child_of(m_scene_root);

		return new_prefab;
	}

	Prefab SceneModule::prefab()
	{
		if (!m_cur_scene_prefab) {
			gz_warn("Prefab does not exist!");
			return Prefab();
		}
		Prefab new_prefab = m_world->prefab()
			.set<TagComponent>({"New Entity"})
			.add<TransformComponent>()
			.child_of(m_cur_scene_prefab);

		if (m_cur_scene)
			m_cur_scene.destruct();

		m_cur_scene = m_world->entity()
			.is_a(m_cur_scene_prefab)
			.child_of(m_scene_root);
		return new_prefab;
	}

	b8 SceneModule::load_scene(Prefab scene_prefab)
	{
		if (!scene_prefab) {
			gz_warn("Scene Prefab is not valid!");
			return false;
		}

		clear_scene();
		m_cur_scene_prefab = scene_prefab;
		m_cur_scene = m_world->entity().is_a(m_cur_scene_prefab).child_of(m_scene_root);
		
		return true;
	}

	Entity SceneModule::lookup(const char* name)
	{
		return m_cur_scene.lookup(name);
	}

	Entity SceneModule::entity(const char *name)
	{
		return m_world->entity(name).child_of(m_cur_scene)
			.set<TagComponent>({name == nullptr ? "New Entity" : name})
			.add<TransformComponent>();
	}

	Entity SceneModule::entity(Entity parent, const char* name)
	{
		if (!parent) {
			gz_warn("Not valid parent");
			return Entity();
		}
		
		return m_world->entity()
			.set<TagComponent>({ name == nullptr ? "New Entity" : name})
			.add<TransformComponent>()
			.child_of(parent);
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
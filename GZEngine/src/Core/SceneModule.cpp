#include <gzpch.h>

#define JSON_USE_IMPLICIT_CONVERSIONS 0
#include <nlohmann/json.hpp>

#include "Project/ProjectModule.h"

#include "SceneModule.h"
#include "TransformModule.h"
#include "MathUtil.h"
#include "FileUtil.h"

#define GZ_SCENE_SERIALIZATION_VERSION 1
#define GZ_SCENE_SERIALIZATION_MAGIC 0x87681426
namespace GZ {
	using json = nlohmann::json;

	void SceneModule::install_into(const ModuleContext& module_ctx)
	{
		// Scene root singleton
		m_world = module_ctx.world;
        m_transform_module = module_ctx.module_reg->get_module<TransformModule>();
        m_project_module = module_ctx.module_reg->get_module<ProjectModule>();
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

		m_cur_scene = m_world->entity().child_of(m_scene_root)
			.set<TagComponent>({ "New Scene" })
			.add<TransformComponent>();
//        load_scene();
//        std::string default_scene_path = m_project_module->get_projcet_dir() + "HelloWorld1.json";
		return m_cur_scene;
	}

    struct SceneMetaData {
        const u32 magic = GZ_SCENE_SERIALIZATION_MAGIC;
        const u32 version = GZ_SCENE_SERIALIZATION_VERSION;
        SceneMetaData() = default;
    };

    void to_json(json& j, const SceneMetaData& scene_meta) {
        j = json{{"magic", scene_meta.magic}, {"version", scene_meta.version}};
    }

    void from_json(const json& j, SceneMetaData& scene_meta) {
        j.at("magic").get_to((u32&)scene_meta.magic);
        j.at("version").get_to((u32&)scene_meta.version);
    }

	Entity SceneModule::load_scene(const std::string &file_path) {
		/*m_cur_scene_prefab = m_world->prefab("Scene")
			.set<TagComponent>({"New Scene"});
			Prefab new_one = m_world->prefab("1").child_of(m_cur_scene_prefab).set<TagComponent>({"New 1"}).slot_of(m_cur_scene_prefab);;
				Prefab new_three = m_world->prefab("3").child_of(new_one).set<TagComponent>({"New 3"}).slot_of(m_cur_scene_prefab);;
					Prefab new_four = m_world->prefab("4").child_of(new_three).set<TagComponent>({"New 4"}).slot_of(m_cur_scene_prefab);;
			Prefab new_two = m_world->prefab("2").child_of(m_cur_scene_prefab).set<TagComponent>({"New 2"}).slot_of(m_cur_scene_prefab);;
				Prefab new_five = m_world->prefab("5").child_of(new_two).set<TagComponent>({"New 5"}).slot_of(m_cur_scene_prefab);;*/
		//gz_info("Create Scene {}", new_three.path().c_str());

		//const char* json_test_entities = R"json(
  //  {
  //    "results": [
		//{"parent":"GZ.SceneRoot", "name":"#754", "components":{"GZ.TransformComponent":{"p":{"x":0, "y":0, "z":0}, "r":{"x":0, "y":0, "z":0, "w":1}, "s":{"x":1, "y":1, "z":1}}, "GZ.TagComponent":{"name":"json_des"}}},
  //      {"parent":"#754", "name":"#888", "components":{"GZ.TransformComponent":{"p":{"x":1, "y":7.2881717682, "z":1}, "r":{"x":0, "y":0, "z":0, "w":1}, "s":{"x":1, "y":1, "z":1}}, "GZ.TagComponent":{"name":"json_des"}}},
  //      {"parent":"#754", "name":"#999", "components":{"GZ.TransformComponent":{"p":{"x":1, "y":7.2881717682, "z":1}, "r":{"x":0, "y":0, "z":0, "w":1}, "s":{"x":1, "y":1, "z":1}}, "GZ.TagComponent":{"name":"json_des2"}}}
  //    ]
  //  })json";
		//m_world->from_json(json_deserialized.c_str(), &desc);

		// This is promising
//        json json_scene;
//        json_scene["entity_names"].push_back("#756");
//        json_scene["entity_names"].push_back("#755");
//        json_scene["entity_names"].push_back("#754");
//		json_scene["entities"].push_back(R"({"parent":"GZ.SceneRoot", "name":"#754", "components":{"GZ.TransformComponent":{"p":{"x":0, "y":0, "z":0}, "r":{"x":0, "y":0, "z":0, "w":1}, "s":{"x":1, "y":1, "z":1}}, "GZ.TagComponent":{"name":"New Scene"}}})");
//		json_scene["entities"].push_back(R"({"parent":"#754", "name":"#755", "components":{"GZ.TransformComponent":{"p":{"x":1, "y":7.2881717682, "z":1}, "r":{"x":0, "y":0, "z":0, "w":1}, "s":{"x":1, "y":1, "z":1}}, "GZ.TagComponent":{"name":"Loaded 1"}}})");
//		json_scene["entities"].push_back(R"({"parent":"#754", "name":"#756", "components":{"GZ.TransformComponent":{"p":{"x":1, "y":7.2881717682, "z":1}, "r":{"x":0, "y":0, "z":0, "w":1}, "s":{"x":1, "y":1, "z":1}}, "GZ.TagComponent":{"name":"Loaded 2"}}})");
//        
//        SceneMetaData scene_meta;
//        json_scene["scene_meta"] = scene_meta;
//		std::string json_scene_dumped = json_scene.dump();
//
//		gz_warn("happy: {}, dumped: {}", json_scene_happy, json_scene_dumped);

        std::string deser_json;
        
        std::string default_scene_path = m_project_module->get_project_dir() + file_path;
        
        FileUtil::read_entire_file(default_scene_path.c_str(), deser_json);
        auto new_json = json::parse(deser_json);
//        auto new_json = json_scene;
        SceneMetaData deser_header = new_json["scene_meta"].template get<SceneMetaData>();
        gz_warn("Loaded scene magic: {}, version: {}", deser_header.magic, deser_header.version);
		struct LoadContext {
            std::unordered_map<std::string, Entity> pre_loaded_entities;
		};
        
		LoadContext ctx;
        for (auto ent_name : new_json["entity_names"]) {
            std::string name = ent_name.template get<std::string>();
            Entity new_ent = m_world->entity();
            ctx.pre_loaded_entities[name] = new_ent;
//            gz_info("QG: {} {}", name, new_ent.name().c_str());
        }
        
        m_cur_scene = ctx.pre_loaded_entities["#754"];
        m_cur_scene.child_of(m_scene_root);
		flecs::from_json_desc_t desc;
		desc.lookup_ctx = &ctx;
		desc.strict = true;
		desc.name = "Load Scene";

		desc.lookup_action = [](const WorldID* world_id, const char* value, void* ctx) -> EntityID {
			gz_info(value);
			World world((WorldID*)world_id);

			LoadContext* load_ctx = (LoadContext*)ctx;
            
            auto it = load_ctx->pre_loaded_entities.find(value);
            if (it != load_ctx->pre_loaded_entities.end()) {
                return it->second.id();
            }

			IdentifierID id = ecs_lookup(world_id, value);

			if (id) {
				return id;
			}

			gz_warn("Reference unknown entity {}", value);
			return 0;

		};

        size_t ent_index = 0;
        
        for (auto pair : ctx.pre_loaded_entities) {
            if (ent_index == 0) {
                // First entity is cur scene root
                m_cur_scene = pair.second;
            }
            std::string json_deserialized = new_json["entities"][ent_index].template get<std::string>();
            ecs_entity_from_json(m_world->get_world(), pair.second.id(), json_deserialized.c_str(), &desc);
        }
        
        m_transform_module->clear_cache();
		return m_cur_scene;
	}

    void SceneModule::iterate_scene_tree_preorder(SceneIterFunc func, void *ctx) {
        private_iterate_scene_tree_preorder(func, ctx, m_scene_root);
    }

    void SceneModule::private_iterate_scene_tree_preorder(SceneIterFunc func, void *ctx, Entity parent) {
        parent.children([func, ctx, parent, this](Entity child) {
            func(m_world, parent, child, ctx);
            private_iterate_scene_tree_preorder(func, ctx, child);
        });
    }

    b8 SceneModule::save_scene(const std::string &file_path) {
        std::string save_path;
        if (file_path.empty()) {
            save_path = m_project_module->get_project_dir() + "testest.gzscn";
        } else {
            save_path = m_project_module->get_project_dir() + file_path;
        }

        SceneMetaData scene_meta;
        json json_scene;
        json_scene["scene_meta"] = scene_meta;
        
        SceneIterFunc add_name = [&](World *world, Entity parent, Entity ent, void *ctx) {
            if (ent.name().size() != 0) {
                json_scene["entity_names"].push_back(ent.name().c_str());
            } else {
                std::string name = "#" + std::to_string(ent.id());
                json_scene["entity_names"].push_back(name.c_str());
            }
        };

        iterate_scene_tree_preorder(add_name, nullptr);
        
        ecs_entity_to_json_desc_t desc = ECS_ENTITY_TO_JSON_INIT;
        desc.serialize_values = true;
        desc.serialize_full_paths = true;
        
        SceneIterFunc ser_entity = [&](World *world, Entity parent, Entity ent, void *ctx) {
            json_scene["entities"].push_back(std::string(ent.to_json(&desc).c_str()));
        };
        
        iterate_scene_tree_preorder(ser_entity, nullptr);

        std::string json_dumped = json_scene.dump();
        return FileUtil::save_file(save_path.c_str(), json_dumped.c_str());
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

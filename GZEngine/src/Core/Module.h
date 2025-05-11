#pragma once
// ECSModule interface declaration
#include <gzpch.h>

#include "Log.h"
struct SDL_Window;

namespace GZ {
	struct SceneModule;
	struct TransformModule;
	struct ComponentRegistry;
	struct ModuleRegistry;
	struct Renderer;
	struct Input;
	struct ModuleContext {
		World *world;
		ComponentRegistry *reg;
		Renderer *renderer;
		Input *input;
		SDL_Window *window;
		ModuleRegistry *module_reg;
	};

	struct Module {
	private:
		static size_t generate_module_id() {
			static size_t id = 0;
			return id++;
		}
	public:
		static const char * invalid_module_name;

		template<typename T>
		static size_t get_module_id() {
			static_assert(std::is_base_of_v<Module, T>);
			static size_t module_type_id = generate_module_id();
			return module_type_id;
		}
		
		template<typename T>
		static const char *get_module_debug_name() {
			static_assert(std::is_base_of_v<Module, T>);
			const char *module_name = typeid(T).name();

			// TODO(Qiming): This is only going to work on MSVC
			for (size_t i = 0; i < std::strlen(module_name); ++i) {
				if (module_name[i] == ' ') return static_cast<const char *>(module_name + (i + 1));
			}

			return module_name;
		}
		
	public:
		virtual ~Module() {};

		// Initialize components, tags, systems, queries
		void virtual install_into(const ModuleContext& module_ctx) = 0;

		// Create some default entities necessary for modules to work
		void virtual after_install(const ModuleContext& module_ctx) { gz_info("{} module: after_install not implemented", m_debug_name); };

		// Get info from other modules, and finish installing
		void virtual end_install(const ModuleContext& module_ctx) { gz_info("{} module: end_install not implemented", m_debug_name); };

		// Clean up resources
		void virtual uninstall_from(const ModuleContext& module_ctx) = 0;

		// This is only used for debug usage
		const char *get_module_debug_name() { return m_debug_name; }
	protected:
		void set_module_debug_name(const char* p_debug_name) {
			m_debug_name = p_debug_name;
		}
	private:
		const char * m_debug_name = invalid_module_name;
	friend class ModuleRegistry;
	};

#define NUM_MAX_ONSTACK_MODULES 200
	struct ModuleRegistry {

		// Singleton module registry, needs to be freed at the end of the program
		static ModuleRegistry *create_global_module_registry();
	public:
		~ModuleRegistry();
		template<typename T>
		size_t add_module() {
			static_assert(std::is_base_of_v<Module, T>);
			size_t generated_module_id = Module::get_module_id<T>();
			gz_assert(generated_module_id == m_num_modules, "generated module id: {} is not the same as m_num_modules: {}", generated_module_id, m_num_modules);

			// Only human readable on MSVC
			const char *module_debug_name = Module::get_module_debug_name<T>();
			T *new_module = new T();
			new_module->set_module_debug_name(module_debug_name);

			if (m_num_modules < NUM_MAX_ONSTACK_MODULES) {
				m_modules[m_num_modules] = new_module;
			}
			else {
				m_extended_modules.push_back(new_module);
				gz_assert(m_extended_modules.size() + NUM_MAX_ONSTACK_MODULES == m_num_modules + 1, "total module numbers are not right");
			}

			return m_num_modules++;
		}

		template<typename T>
		T *get_module() {
			static_assert(std::is_base_of_v<Module, T>);
			size_t index = Module::get_module_id<T>();
			gz_assert(m_num_modules > index, "no modules or invalid index");
			if (index < NUM_MAX_ONSTACK_MODULES) {
				return static_cast<T *>(m_modules[index]);
			}
			else {
				return static_cast<T *>(m_extended_modules[index - NUM_MAX_ONSTACK_MODULES]);
			}
		}

		template<typename T>
		T* get_module_by_name() {
			size_t num_on_stack_modules = std::fmin(m_num_modules, NUM_MAX_ONSTACK_MODULES);
			const char *name = Module::get_module_debug_name<T>();
			for (size_t i = 0; i < num_on_stack_modules; ++i) {
				if (std::strcmp(name, m_modules[i]->get_module_debug_name()) == 0) {
					return static_cast<T *>(m_modules[i]);
				}
			}

			if (m_num_modules > NUM_MAX_ONSTACK_MODULES) {
				for (size_t i = 0; i < m_extended_modules.size(); ++i) {
					if (std::strcmp(name, m_extended_modules[i]->get_module_debug_name()) == 0) {
						return static_cast<T*>(m_extended_modules[i]);
					}
				}
			}

			return nullptr;
		}

		b8 install_all_modules(const ModuleContext &module_ctx);
		b8 after_install_all_modules(const ModuleContext &module_ctx);
		b8 end_install_all_modules(const ModuleContext &module_ctx);
		b8 uninstall_all_modules(const ModuleContext &module_ctx);
		
		
	protected:
		ModuleRegistry();
		
	private:
		std::array<Module *, NUM_MAX_ONSTACK_MODULES> m_modules;
		std::vector<Module *> m_extended_modules;
		size_t m_num_modules = 0;
	};
}

// Component formatter
template<>
struct fmt::formatter<flecs::string> : fmt::formatter<std::string>
{
	auto format(flecs::string &my_flecs_string, format_context& ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "{}", my_flecs_string.c_str());
	}
};
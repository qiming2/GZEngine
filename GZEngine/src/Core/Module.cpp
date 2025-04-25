#include "gzpch.h"
#include "Module.h"
#include "Log.h"

namespace GZ {
	const char* Module::invalid_module_name = "NONE";

	ModuleRegistry *ModuleRegistry::get_module_registry()
	{
		static b8 is_initialized = false;
		static ModuleRegistry * g_module_registry = nullptr;
		if (is_initialized) return g_module_registry;
		is_initialized = true;
		g_module_registry = new ModuleRegistry;
		return g_module_registry;
	}

	b8 ModuleRegistry::install_all_modules(const ModuleContext &module_ctx)
	{
		size_t num_on_stack_modules = std::fmin(m_num_modules, NUM_MAX_ONSTACK_MODULES);
		
		for (size_t i = 0; i < num_on_stack_modules; ++i) {
			m_modules[i]->install_into(module_ctx);
		}

		if (m_num_modules > NUM_MAX_ONSTACK_MODULES) {
			for (size_t i = 0; i < m_extended_modules.size(); ++i) {
				m_extended_modules[i]->install_into(module_ctx);
			}
		}

		return true;
	}

	b8 ModuleRegistry::after_install_all_modules(const ModuleContext &module_ctx)
	{
		size_t num_on_stack_modules = std::fmin(m_num_modules, NUM_MAX_ONSTACK_MODULES);

		for (size_t i = 0; i < num_on_stack_modules; ++i) {
			m_modules[i]->after_install(module_ctx);
		}

		if (m_num_modules > NUM_MAX_ONSTACK_MODULES) {
			for (size_t i = 0; i < m_extended_modules.size(); ++i) {
				m_extended_modules[i]->after_install(module_ctx);
			}
		}

		return true;
	}

	b8 ModuleRegistry::uninstall_all_modules(const ModuleContext &module_ctx)
	{
		size_t num_on_stack_modules = std::fmin(m_num_modules, NUM_MAX_ONSTACK_MODULES);

		for (size_t i = 0; i < num_on_stack_modules; ++i) {
			m_modules[i]->uninstall_from(module_ctx);
		}

		if (m_num_modules > NUM_MAX_ONSTACK_MODULES) {
			for (size_t i = 0; i < m_extended_modules.size(); ++i) {
				m_extended_modules[i]->uninstall_from(module_ctx);
			}
		}

		return true;
	}

	ModuleRegistry::ModuleRegistry()
	{
		
	}

	ModuleRegistry::~ModuleRegistry()
	{
		size_t num_on_stack = std::fmin(m_num_modules, NUM_MAX_ONSTACK_MODULES);
		for (size_t i = 0; i < num_on_stack; ++i) {

			delete m_modules[i];
			m_modules[i] = nullptr;
		}

		for (size_t i = 0; i < m_extended_modules.size(); ++i) {
			delete m_extended_modules[i];
		}
		m_extended_modules.clear();
		m_num_modules = 0;
	}

}

#pragma once
// ECSModule interface declaration
#include <gzpch.h>
#include "Renderer.h"
#include "Input.h"

namespace GZ {

	struct ComponentRegistry;

	struct ModuleContext {
		World *world;
		ComponentRegistry *reg;
		Renderer *renderer;
		Input *input;
		SDL_Window* window;
	};
	struct Module {

		void virtual install_into(const ModuleContext& module_ctx) = 0;

		void virtual uninstall_from(const ModuleContext& module_ctx) = 0;
	};
}
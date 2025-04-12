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

		void virtual install_into(World &world, ComponentRegistry &reg) = 0;

		void virtual uninstall_from(World &world, ComponentRegistry &reg) = 0;

		void virtual pass_context(ModuleContext &ctx) = 0;
	};
}
#pragma once
#include <defines.h>
#include <imgui.h>
#include <flecs.h>
#include <SDL3/SDL.h>

#include <imgui_node_editor.h>

#include <string>

#include "Renderer.h"
namespace GZ {
	struct AppSpec {
		b8 headless = false;
		u32 window_height = 1960;
		u32 window_width = 1080;
		std::string name = "New App";
	};

	namespace ed = ax::NodeEditor;
	struct App {
	public:
		explicit App(const AppSpec &spec);
		virtual ~App();

		void run();
	protected:
		b8 is_initialized = false;

		// flecs ecs
		flecs::world world;

		// Renderer
		Renderer *vk_renderer = nullptr;
		// TODO(Qiming): temp var in editor app
		bool show_demo_window = true;
		bool show_another_window = true;
		bool show_node_editor = true;
		ImVec4 clear_color = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
		SDL_GLContext gl_context;

		ed::EditorContext* node_Context = nullptr;

		// Asset
		std::string working_dir;

		// window
		SDL_Window* window = nullptr;
		bool is_running = true;
	};
	
	
}
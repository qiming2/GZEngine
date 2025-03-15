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
		u32 window_height = 1080;
		u32 window_width = 1960;
		b8 is_fullscreen = false;
		std::string name = "New App";
	};

	struct FrameData {
		f32 deltaTime;
		u64 prevTime; // in ns
	};

	namespace ed = ax::NodeEditor;
	struct App {
	public:
		explicit App(const AppSpec &spec);
		virtual ~App();

		void run();
		
		void onMainThreadBlock();
		
	protected:
		

		b8 is_initialized = false;
		b8 is_fullscreen = false;
		SDL_EventFilter expose_event_watch;

		FrameData frame_data;

		// flecs ecs
		flecs::world world;

		// Renderer
		Renderer *vk_renderer = nullptr;
		// TODO(Qiming): temp var in editor app
		b8 show_demo_window = true;
		b8 show_another_window = true;
		b8 show_node_editor = true;
		b8 show_main_scene = true; 
		ImTextureID main_tex_id;

		vec4 clear_color = vec4(0.18f, 0.18f, 0.18f, 1.00f);
		SDL_GLContext gl_context;

		ed::EditorContext* node_Context = nullptr;

		// Asset
		std::string working_dir;

		// window
		SDL_Window* window = nullptr;
		bool is_running = true;

		u32 main_view_w = 0, main_view_h = 0;
		u32 window_w = 1960, window_h = 1080;
	private:
		void resize();
		void pre_render();
		void post_render();
		void render_editor();
	};

	
}
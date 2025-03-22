#pragma once
#include <defines.h>
#include <imgui.h>
#include <flecs.h>
#include <SDL3/SDL.h>

#include <imgui_node_editor.h>
#include <string>
#include <cr.h>

#include "Renderer.h"
#include "Log.h"
#include "PhysicsModule.h"
#include "ComponentInterface.h"
#include "Profiler.h"

namespace GZ {
	struct AppSpec {
		b8 headless = false;
		u32 window_height = 1080;
		u32 window_width = 1960;
		b8 is_fullscreen = false;
		b8 is_hot_reloading = true;
		std::string name = "New App";
	};

	struct FrameData {
		f32 deltaTime;
		u64 prevTime; // in ns
	};

	namespace ed = ax::NodeEditor;
	struct EditorContext {
		World *world;
		ComponentRegistry *reg;
		ImGuiContext* imgui_ctx;
		Renderer* gz_renderer;
		FrameData frame_data;

		b8 m_show_demo_window = true;
		b8 m_show_another_window = true;
		b8 m_show_node_editor = true;
		b8 m_show_main_scene = true;

		u32 m_main_view_w = 0, m_main_view_h = 0;
		ImTextureID main_tex_id;
		vec4 m_clear_color = vec4(0.18f, 0.18f, 0.18f, 1.00f);

		ed::EditorContext* m_node_Context = nullptr;
	};


	struct App {
	public:
		GZ_API explicit App(const AppSpec &spec);
		GZ_API virtual ~App();
    
    public: // Non virtual public
		
        GZ_API void run();
        GZ_API void loop();
	protected:
		// Temp Renderer Stuff
		Renderer* gz_renderer = nullptr;

		// flecs ecs temporary prototyping
		World world;
		ComponentRegistry reg;
        
        // expose physics for prototyping
        PhysicsModule physics_module;

	private: // editor stuff
		void private_on_imgui_render();
		
		EditorContext plugin_data;

		
	private:
		// plugin
		cr_plugin ctx;

		// Profiler
		std::shared_ptr<Profiler> m_profiler_instance_ref;

		b8 is_fullscreen = false;
		SDL_EventFilter expose_event_watch;

		FrameData m_frame_data;

		// Asset
		std::string working_dir;

		// window
		SDL_Window* window = nullptr;
		bool is_running = true;

		u32 window_w = 1960, window_h = 1080;
	private:
        b8 is_app_initialized = false;
        void private_render();
		void private_resize();
		void private_pre_render();
		void private_post_render();
		void private_install_builtin_modules();
	};

	
}

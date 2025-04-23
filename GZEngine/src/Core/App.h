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
#include "RenderModule.h"
#include "ComponentInterface.h"
#include "Profiler.h"
#include "Input.h"

#include "GameModule.h"

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
		Renderer *gz_renderer;
		Profiler *profiler;
        Input *input;
		FrameData frame_data;
        SDL_Window *window;
        ed::EditorContext* m_node_context = nullptr;
        ImTextureID main_tex_id;

		// Modules for tweaking module related settings
		PhysicsModule *physics_module;
		RenderModule *render_module;

		ModuleContext *module_ctx;
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
		ModuleContext m_module_ctx;
        
        // expose physics for prototyping
        PhysicsModule m_physics_module;
		CommonModule m_common_module;
		RenderModule m_render_module;

		// Game modules
		GameModule m_game_module;
	private: // editor stuff
		EditorContext plugin_data;

		
	private:
		// plugin
		cr_plugin ctx;

		// Profiler
		Profiler *m_profiler;
        
        Input *m_input;

		b8 is_fullscreen = false;
		SDL_EventFilter expose_event_watch;

		FrameData m_frame_data;

		// Asset
		std::string working_dir;

		// window
		SDL_Window* window = nullptr;
		bool is_running = true;

		u32 window_w = 1960, window_h = 1080;
		void private_setup_initial_scene();
	private:
        void private_end_render_frame();
		void private_resize();
        void private_begin_frame();
        void private_end_frame();
		void private_begin_render_frame();
		void private_install_builtin_modules();
		void private_install_game_modules();
	};

	
}

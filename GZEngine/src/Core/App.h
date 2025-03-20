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
namespace GZ {
	struct GZ_API AppSpec {
		b8 headless = false;
		u32 window_height = 1080;
		u32 window_width = 1960;
		b8 is_fullscreen = false;
		b8 is_hot_reloading = true;
		std::string name = "New App";
	};

	struct GZ_API FrameData {
		f32 deltaTime;
		u64 prevTime; // in ns
	};

	struct PluginData {
		ImGuiContext* imgui_ctx;
		FrameData frame_data;
	};

	namespace ed = ax::NodeEditor;
	struct GZ_API App {
	public:
		explicit App(const AppSpec &spec);
		virtual ~App();
        virtual inline void on_init() { gz_core_warn("App should implement this!");}
        virtual inline void on_update(const FrameData &frame_data) { gz_core_warn("App should implement this!");}
        void on_imgui_render();
    
    public: // Non virtual public
        void run();
        void loop();
	protected:
		// Temp Renderer Stuff
		Renderer* gz_renderer = nullptr;
		ImTextureID main_tex_id;

		// flecs ecs temporary prototyping
		World world;
        
        // expose physics for prototyping
        PhysicsModule physics_module;

	private: // editor stuff
		
		
		PluginData plugin_data;

		b8 m_show_demo_window = true;
		b8 m_show_another_window = true;
		b8 m_show_node_editor = true;
		b8 m_show_main_scene = true;

		u32 m_main_view_w = 0, m_main_view_h = 0;

		vec4 m_clear_color = vec4(0.18f, 0.18f, 0.18f, 1.00f);

		ed::EditorContext* m_node_Context = nullptr;
	private:
		// plugin
		cr_plugin ctx;

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

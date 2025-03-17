#pragma once
#include <defines.h>
#include <imgui.h>
#include <flecs.h>
#include <SDL3/SDL.h>

#include <imgui_node_editor.h>

#include <string>

#include "Renderer.h"
#include "Log.h"
#include "PhysicsModule.h"

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
        virtual inline void on_init() { gz_core_warn("App should implement this!");}
        virtual inline void on_update(const FrameData &frame_data) { gz_core_warn("App should implement this!");}
        virtual inline void on_imgui_render() { gz_core_warn("App should implement this!");}
    
    public: // Non virtual public
        void run();
        void loop();
	protected:
		// Temp Renderer Stuff
		Renderer* vk_renderer = nullptr;
		ImTextureID main_tex_id;

		// flecs ecs temporary prototyping
		flecs::world world;
        
        // expose physics for prototyping
        PhysicsModule physics_module;
	private:

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
	};

	
}

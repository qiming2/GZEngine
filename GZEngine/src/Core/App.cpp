#include <gzpch.h>
#define CR_HOST CR_UNSAFE
#include <cr.h>
#include <App.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <imgui.h>
#include <flecs.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_vulkan.h>
#include <imgui_stdlib.h>
#include <filesystem>

#include <vulkan/vulkan.h>
#include <imgui.h>
#include <flecs.h>
#include <glm/glm.hpp>

#include <imgui_node_editor.h>
#include <defines.h>

#include "Log.h"
#include "FileUtil.h"
#include "MathUtil.h"
#include "Input.h"
#include "ImGuiUtil.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderModule.h"
#include "Physics/PhysicsModule.h"
#include "Common/CommonModule.h"


#define USE_IMGUI 1

namespace GZ {

	namespace ed = ax::NodeEditor;
	App::App(const AppSpec& spec)
	{
		GZ::Log::init();

        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS)) {
			gz_error("SDL init failed: {}", SDL_GetError());
			exit(1);
		}

		//Profiler
		GZ::Profiler::init();
		m_profiler = &GZ::Profiler::g_profiler_instance;

        GZ::Input::init();
        m_input = &GZ::Input::g_input_instance;
        
		char* wkd = SDL_GetCurrentDirectory();

		// on windows //, other platform
		working_dir = wkd;
		SDL_free((void*)wkd);

		this->is_fullscreen = spec.is_fullscreen;

		SDL_WindowFlags sdl_resizable_flag = 0;
#ifndef GZ_DIST
		sdl_resizable_flag = SDL_WINDOW_RESIZABLE;
#endif

		SDL_WindowFlags window_flags = SDL_WINDOW_VULKAN | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_HIDDEN | sdl_resizable_flag;
		window = SDL_CreateWindow("GZ Editor", spec.window_width, spec.window_height, window_flags);
		if (window == nullptr) {
			gz_error("SDL Create window Failed!: {}", SDL_GetError());
			exit(1);
		}
		
		// TODO(Qiming)(VULKAN)
		gz_renderer = new Renderer();
		gz_renderer->init((void *)window, world);
        
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

		SDL_ShowWindow(window);
        SDL_SetWindowFullscreen(window, is_fullscreen);

		// Imgui setup
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

#ifdef GZ_PLATFORM_WINDOWS
		std::string path = working_dir + std::string("asset\\font\\NotoSansTC-Medium.ttf");
#elif GZ_PLATFORM_APPLE
		std::string path = working_dir + std::string("asset/font/NotoSansTC-Medium.ttf");
#endif

		gz_info("Loading new font from {}", path);
		io.Fonts->AddFontFromFileTTF(path.c_str(), 22);
		// Setup Dear ImGui style
		//ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();
		SetupImGuiStyle();
		
		// Setup Platform/Renderer backends
		ImGui_ImplSDL3_InitForVulkan(window);
		ImGui_ImplVulkan_InitInfo init_info = {};
		gz_renderer->init_imgui_vulkan(&init_info);
		ImGui_ImplVulkan_Init(&init_info);

		// Load Main texture
		plugin_data.main_tex_id = (ImTextureID)gz_renderer->get_main_color_texture_imgui_id();

		// Node editor
		ed::Config config;
		config.SettingsFile = "Simple.json";
		plugin_data.m_node_Context = ed::CreateEditor(&config);

        //// Setup physics engine
        //physics_module.init();
        //physics_module.create_default_objects();

        m_frame_data.prevTime = SDL_GetTicksNS();
        m_frame_data.deltaTime = 0.0f;

		// Temp add sphere
        auto sphere_mesh = Mesh::get_icosphere_mesh(0.5f);
        auto box_mesh = Mesh::get_box_mesh();
        gz_renderer->submit_mesh(sphere_mesh);
        gz_renderer->submit_mesh(box_mesh);
		
		// plugin system
#ifdef GZ_PLATFORM_APPLE
        const char *plugin_path = "/Users/qimingguan/qiming/GZEngine/build/GZEditor/EditorHotReload/Debug/libGZEditorHotReload.dylib";
#else
        const char *plugin_path = "E:\\GZEngine\\build\\bin\\Debug\\GZEditorHotReload.dll";
#endif
		// init some plugin data so plugin data
		plugin_data.world = &world;
		plugin_data.reg = &reg;
		plugin_data.imgui_ctx = ImGui::GetCurrentContext();
		plugin_data.gz_renderer = gz_renderer;
		plugin_data.profiler = m_profiler;
		ctx.userdata = &plugin_data;
		
		cr_plugin_open(ctx, plugin_path);
		
		SDL_EventFilter expose_event_watch = [](void* usr_data, SDL_Event* event) -> b8 {
			App* app = (App*)usr_data;

			if (event->type == SDL_EVENT_WINDOW_EXPOSED) {
				
				app->loop();
			}
			return true;
		};
		SDL_AddEventWatch(expose_event_watch, this);

		// Init all ecs modules, systems, components
		// Before runnning, we install builtin ecs modules
		world.set_threads(4);
		private_install_builtin_modules();
		
        
        // deserilize or create/install default entities to world
		// Create some ents to test plugin ecs module
        
        // Camera component
        auto camera_e = world.entity("Camera")
            .set<CameraComponent>({GZ_PI * 0.25f, static_cast<f32>(spec.window_width / spec.window_height), 0.1f, 100.0f, true, true})
            .set<TransformComponent>({.p = vec3{0.0, 0.0, 2.0}});
        
        auto e1 = world.entity("Hello").set<TransformComponent>({vec3{1.0, 1.0, 1.0}, quat{1, 0, 0, 0}, vec3{1.0, 1.0, 1.0}});

		e1.set<RigidbodyComponent>({physics_module.m_sphere_id});
        e1.set<MeshComponent>({sphere_mesh});
        auto e2 = world.entity("Hello1").set<TransformComponent>({vec3{2.0, 2.0, 2.0}, quat{1, 0, 0, 0}, vec3{1.0, 1.0, 1.0}});
		e2.set<RigidbodyComponent>({physics_module.m_box_id});
        e2.set<MeshComponent>({box_mesh});
		
		std::shared_ptr<Mesh> model_mesh = Mesh::load_mesh_from_obj("asset/model/meng_yuan.obj");
		gz_renderer->submit_mesh(model_mesh);
		auto e3 = world.entity("Player")
			.set<TransformComponent>({vec3{0.0, 0.0, 0.0}, quat{1, 0, 0, 0}, vec3{1.0, 1.0, 1.0}})
			.set<MeshComponent>({model_mesh});

		static f32 rotate_scale = 2.0f;
		world.system("Rotate Character")
			.write<TransformComponent>()
			.run([&](WorldIter& it) {
			auto player = world.lookup("Player");
			TransformComponent *t = player.get_mut<TransformComponent>();
			quat cur = t->r;
			quat rotate = glm::angleAxis(GZ_PI * rotate_scale * it.delta_time(), vec3{ 0, 1, 0 });
			cur = glm::normalize(cur * rotate);
			//cur = glm::rotate(cur, it.delta_time() * rotate_scale, vec3{0, 1, 0});
			t->r = cur;

			//t->r = vec3{0, t->r.y + it.delta_time() * 45.0f, 0};
		});
        
	}

	App::~App()
	{
		cr_plugin_close(ctx);
        physics_module.deinit();
        
		SDL_RemoveEventWatch(expose_event_watch, this);
		gz_renderer->will_deinit();
		// Cleanup

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();

		gz_renderer->deinit();
		delete gz_renderer;
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void App::run() {
        
		SDL_Event e;
		u64 prevTime = SDL_GetTicksNS();
		ImGuiIO& io = ImGui::GetIO();
		while (is_running) {
            // Setup new frames
            
            // Input first, we might want imgui to capture events
            // so we call pre_render to get start new frames for imgui
            // and renderer
            
			
			while (SDL_PollEvent(&e)) {
				switch (e.type) {
				case SDL_EVENT_QUIT:
					gz_info("Quiting...");
					is_running = false;
					break;
				case SDL_EVENT_KEY_DOWN:
					switch (e.key.key) {
					case SDLK_ESCAPE:
						is_running = false;
                        continue;
                        break;
					case SDLK_P:
						is_fullscreen = !is_fullscreen;
						SDL_SetWindowFullscreen(window, is_fullscreen);
                        continue;
                        break;
					}
					break;
				}
                // Pass inputs to input system
                // Input should be just handled
                // through GetKeyboard state...
                
                
				ImGui_ImplSDL3_ProcessEvent(&e);
				if (io.WantCaptureKeyboard || io.WantCaptureMouse) {
					continue;
				}
                
				switch (e.type) {
				case SDL_EVENT_MOUSE_WHEEL:

					m_input->add_input_event(e);
					continue;
					break;
				}
			}
            
			if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
            {
                SDL_Delay(10);
                continue;
            }
            
            loop();
		}
        

	}

	void App::private_resize()
	{
		/*gz_info("Window pixel size has changed, need to signal renderer to recreate swapchain...");*/
		int t_w = 0, t_h = 0;
		SDL_GetWindowSizeInPixels(window, (int*)&t_w, (int*)&t_h);
		if (t_w != window_w || t_h != window_h) {
			window_h = t_h;
			window_w = t_w;
			gz_renderer->handle_window_resized();
            ImGui_ImplVulkan_SetMinImageCount(gz_renderer->get_min_image_count());
			ImGui_ImplVulkan_RemoveTexture((VkDescriptorSet)plugin_data.main_tex_id);
			plugin_data.main_tex_id = (ImTextureID)gz_renderer->get_main_color_texture_imgui_id();
		}
	}

	void App::loop()
	{
        
        // Even main_thread_blocked, we still want to run update and render
        // no events
		private_resize();
        
        // Maybe will stop rendering the frame if the window
        // is minimized
        private_begin_frame();
        private_begin_render_frame();

		cr_plugin_update(ctx);

        {
            ScopedProfiler world_progress("World Progress");
            world.progress(m_frame_data.deltaTime);
        }

        private_end_render_frame();
        private_end_frame();
	}

    void App::private_begin_frame() {
        u64 curTime = SDL_GetTicksNS();
        m_frame_data.deltaTime = static_cast<f32>((f64)(curTime - m_frame_data.prevTime) / (f64)SDL_NS_PER_SECOND);
        m_frame_data.prevTime = curTime;
        
        //patch EditorData
        plugin_data.frame_data = m_frame_data;
        m_profiler->begin_frame();
		m_input->begin_frame();
    }

	void App::private_begin_render_frame()
	{
		
        gz_renderer->begin_frame(m_frame_data.deltaTime);
        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

		// put dockspace here so everything is docked
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
	}

	void App::private_end_frame()
	{
		m_profiler->end_frame();

		f64 per_frame_ms = m_profiler->get_last_per_frame_data().measured_time / (f64)SDL_NS_PER_MS;
		u64 fps = static_cast<u64>(floor(1.0 / per_frame_ms * 1000.0));

	}

	void App::private_install_builtin_modules()
	{
		// Modules can be plugin
		CommonModule common_module;
		common_module.install_into(world, reg);
		physics_module.install_into(world, reg);
		physics_module.create_default_objects();
        RenderModule render_module;
        render_module.install_into(world, reg);
	}

	void App::private_end_render_frame()
	{

		// Rendering
        ImGui::EndFrame();

        ImGui::Render();
        
		ImDrawData* main_draw_data = ImGui::GetDrawData();

		gz_renderer->set_imgui_draw_data(main_draw_data);
		gz_renderer->end_frame();
        
	}

}

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
#include "Renderer/Renderer.h"
#include "Physics/PhysicsModule.h"
#include "Common/CommonModule.h"

#define USE_IMGUI 1

namespace GZ {

	// Add another style maybe
	static void SetupImGuiStyle()
	{
		// Primary style from ImThemes
		ImGuiStyle& style = ImGui::GetStyle();

		style.Alpha = 1.0f;
		style.DisabledAlpha = 1.0f;
		style.WindowPadding = ImVec2(10.0f, 10.0f);
		style.WindowRounding = 0.0f;
		style.WindowBorderSize = 0.0f;
		style.WindowMinSize = ImVec2(20.0f, 20.0f);
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_None;
		style.ChildRounding = 0.0f;
		style.ChildBorderSize = 0.0f;
		style.PopupRounding = 3.0f;
		style.PopupBorderSize = 0.0f;
		style.FramePadding = ImVec2(10.0f, 5.0f);
		style.FrameRounding = 3.0f;
		style.FrameBorderSize = 0.0f;
		style.ItemSpacing = ImVec2(6.0f, 6.0f);
		style.ItemInnerSpacing = ImVec2(6.0f, 3.0f);
		style.CellPadding = ImVec2(6.0f, 10.0f);
		style.IndentSpacing = 20.0f;
		style.ColumnsMinSpacing = 6.0f;
		style.ScrollbarSize = 12.0f;
		style.ScrollbarRounding = 2.0f;
		style.GrabMinSize = 7.0f;
		style.GrabRounding = 1.0f;
		style.TabRounding = 3.0f;
		style.TabBorderSize = 0.0f;
		style.TabCloseButtonMinWidthUnselected = 0.0f;
		style.ColorButtonPosition = ImGuiDir_Right;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

#if 1
		// in srgb space
		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.249111220240593f, 0.3344880044460297f, 0.4206008315086365f, 1.0f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.07058823853731155f, 0.07058823853731155f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.1094144284725189f, 0.1320907771587372f, 0.1416308879852295f, 1.0f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1176470592617989f, 0.1176470592617989f, 0.1176470592617989f, 1.0f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3882353007793427f, 0.1522491276264191f, 0.1522491276264191f, 1.0f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.6784313917160034f, 0.2660515010356903f, 0.2660515010356903f, 1.0f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.04291844367980957f, 0.04236584529280663f, 0.04236584529280663f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9882352948188782f, 0.3875432312488556f, 0.3875432312488556f, 1.0f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.9882352948188782f, 0.3882353007793427f, 0.3882353007793427f, 1.0f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.9999899864196777f, 0.9999899864196777f, 1.0f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3882353007793427f, 0.1529411822557449f, 0.1529411822557449f, 1.0f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.6784313917160034f, 0.2666666805744171f, 0.2666666805744171f, 1.0f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.3882353007793427f, 0.1529411822557449f, 0.1529411822557449f, 1.0f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.6784313917160034f, 0.2666666805744171f, 0.2666666805744171f, 1.0f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.3882353007793427f, 0.1529411822557449f, 0.1529411822557449f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.6784313917160034f, 0.2666666805744171f, 0.2666666805744171f, 1.0f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.3882353007793427f, 0.1529411822557449f, 0.1529411822557449f, 1.0f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.6784313917160034f, 0.2666666805744171f, 0.2666666805744171f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.4134539067745209f, 0.7467811107635498f, 0.7339057922363281f, 1.0f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.7669611573219299f, 0.7882353067398071f, 0.4265743792057037f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.9847311973571777f, 1.0f, 0.7124463319778442f, 1.0f);
		style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.6784313917160034f, 0.2666666805744171f, 0.2666666805744171f, 1.0f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.9882352948188782f, 0.3882353007793427f, 0.3882353007793427f, 1.0f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.9882352948188782f, 0.3882353007793427f, 0.3882353007793427f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.9882352948188782f, 0.3882353007793427f, 0.3882353007793427f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.9882352948188782f, 0.3882353007793427f, 0.3882353007793427f, 0.501960813999176f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.9882352948188782f, 0.3882353007793427f, 0.3882353007793427f, 0.501960813999176f);
#else
		// in linear space
		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.051269f, 0.089437f, 0.141630f, 1.0f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.005497f, 0.005497f, 0.005497f, 1.0f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.007768f, 0.008856f, 0.012038f, 1.0f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.007768f, 0.008856f, 0.012038f, 1.0f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.012038f, 0.016807f, 0.019349f, 1.0f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.007768f, 0.008856f, 0.012038f, 1.0f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.012038f, 0.012038f, 0.012038f, 1.0f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.117647f, 0.020661f, 0.020661f, 1.0f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.385499f, 0.054902f, 0.054902f, 1.0f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.003906f, 0.004764f, 0.007768f, 1.0f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.003906f, 0.004764f, 0.007768f, 1.0f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.007768f, 0.008856f, 0.012038f, 1.0f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.009766f, 0.010768f, 0.013856f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.002130f, 0.002089f, 0.002089f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.012038f, 0.015686f, 0.020661f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.020661f, 0.024771f, 0.033104f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.012038f, 0.015686f, 0.020661f, 1.0f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.970085f, 0.117647f, 0.117647f, 1.0f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.970085f, 0.117647f, 0.117647f, 1.0f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.012038f, 0.015686f, 0.020661f, 1.0f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.117647f, 0.020661f, 0.020661f, 1.0f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.385499f, 0.054902f, 0.054902f, 1.0f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.012038f, 0.015686f, 0.020661f, 1.0f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.117647f, 0.020661f, 0.020661f, 1.0f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.385499f, 0.054902f, 0.054902f, 1.0f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.020661f, 0.027451f, 0.050980f, 1.0f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.020661f, 0.027451f, 0.050980f, 1.0f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.020661f, 0.027451f, 0.050980f, 1.0f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.012038f, 0.015686f, 0.020661f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.117647f, 0.020661f, 0.020661f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.385499f, 0.054902f, 0.054902f, 1.0f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.003906f, 0.004764f, 0.007768f, 1.0f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.117647f, 0.020661f, 0.020661f, 1.0f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.385499f, 0.054902f, 0.054902f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.003906f, 0.004764f, 0.007768f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.007768f, 0.008856f, 0.012038f, 1.0f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.141630f, 0.478431f, 0.466667f, 1.0f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.003130f, 0.960784f, 0.960784f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.537254f, 0.564706f, 0.152941f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.970085f, 1.0f, 0.466667f, 1.0f);
		style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.003906f, 0.004764f, 0.007768f, 1.0f);
		style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.003906f, 0.004764f, 0.007768f, 1.0f);
		style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.012038f, 0.015686f, 0.020661f, 1.0f);
		style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.009766f, 0.010768f, 0.013856f, 1.0f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.385499f, 0.054902f, 0.054902f, 1.0f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.970085f, 0.117647f, 0.117647f, 1.0f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.970085f, 0.117647f, 0.117647f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.970085f, 0.117647f, 0.117647f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.970085f, 0.117647f, 0.117647f, 0.501961f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.970085f, 0.117647f, 0.117647f, 0.501961f);
#endif

		// Custom ones that are not exported from Imthemes
		style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
		style.ImageBorderSize = 3.0f;
		style.TabBarOverlineSize = 0.0f;
		style.TabBarBorderSize = 1.0f;
	}

	namespace ed = ax::NodeEditor;
	App::App(const AppSpec& spec)
	{
		GZ::Log::init();
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS)) {
			gz_error("SDL init failed: {}", SDL_GetError());
			exit(1);
		}

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
		gz_renderer->init((void *)window);
        
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
		main_tex_id = (ImTextureID)gz_renderer->get_main_color_texture_imgui_id();

		// Node editor
		ed::Config config;
		config.SettingsFile = "Simple.json";
		m_node_Context = ed::CreateEditor(&config);

        // Setup physics engine
        physics_module.init();
        physics_module.create_default_objects();

        m_frame_data.prevTime = SDL_GetTicksNS();
        m_frame_data.deltaTime = 0.0f;

		// Temp add sphere
		gz_renderer->submit_mesh(Mesh::get_icosphere_mesh(0.5f));
        gz_renderer->submit_mesh(Mesh::get_box_mesh());

		// Before runnning, we install builtin ecs modules
		private_install_builtin_modules();

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

		// Create some ents to test plugin ecs module
		auto e1 = world.entity("Hello").set<TransformComponent>({vec3{1.0, 1.0, 1.0}, vec4{2.0, 2.0, 2.0, 1.0}, vec3{3.0, 3.0, 3.0}});
		
		auto elook = world.lookup("Hello");
		
		auto e = world.entity("Hello1");
		
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

	void App::private_on_imgui_render()
	{
		ImGuiIO& io = ImGui::GetIO();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open", "CTRL+Z")) {}
				if (ImGui::MenuItem("Hello", "CTRL+Z")) {}
				ImGui::EndMenu();

			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {} // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}


		if (m_show_node_editor) {
			ImGui::Begin("New editor window");
			ed::SetCurrentEditor(m_node_Context);
			ed::Begin("My Editor", ImVec2(0.0, 0.0f));
			int uniqueId = 1;
			// Start drawing nodes.
			ed::BeginNode(uniqueId++);
			ImGui::Text("Node A");
			ed::BeginPin(uniqueId++, ed::PinKind::Input);
			ImGui::Text("-> In");
			ed::EndPin();
			ImGui::SameLine();
			ed::BeginPin(uniqueId++, ed::PinKind::Output);
			ImGui::Text("Out ->");
			ed::EndPin();
			ed::EndNode();
			ed::End();
			ed::SetCurrentEditor(nullptr);
			ImGui::End();
		}


		if (m_show_demo_window)
			ImGui::ShowDemoWindow(&m_show_demo_window);

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &m_show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &m_show_another_window);
			ImGui::Checkbox("Node editor", &m_show_node_editor);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			if (ImGui::ColorEdit3("clear color", (float*)&m_clear_color)) {
				gz_renderer->set_clear_value(m_clear_color);
			} // Edit 3 floats representing a color

			static TransformComponent comp;
			// Test component draw ui
			/*draw_component_imgui_ui_transform(&comp);*/

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.5f ms/frame (%.2f FPS)", m_frame_data.deltaTime, 1.0f / m_frame_data.deltaTime);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (m_show_another_window)
		{
			ImGui::Begin("Another Window", &m_show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				m_show_another_window = false;

			ImGui::End();
		}

		// 4. Show main shaing
		if (m_show_main_scene) {

			// rethink ui design later
//            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });

//            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse;
			ImGui::Begin("Main Scene", &m_show_main_scene, 0);
			ImGui::SetWindowSize({ 200.0f, 200.0f });

			// Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImVec2 main_scene_cur_window_size = ImGui::GetContentRegionAvail();
			if (m_main_view_w != static_cast<u32>(main_scene_cur_window_size.x) || m_main_view_h != static_cast<u32>(main_scene_cur_window_size.y)) {
				m_main_view_w = static_cast<u32>(main_scene_cur_window_size.x);
				m_main_view_h = static_cast<u32>(main_scene_cur_window_size.y);
				gz_renderer->set_viewport_size(m_main_view_w, m_main_view_h);
			}

			ImGui::PushStyleVar(ImGuiStyleVar_ImageBorderSize, 0.0f);
			ImGui::Image((ImTextureID)main_tex_id, main_scene_cur_window_size, { 0, 0 }, { 1, 1 });
			ImGui::PopStyleVar();

			ImGui::End();
			//            ImGui::PopStyleVar();
		}
	}

	void App::run() {
		
		// Then do app init, app can install its custom module
        if (!is_app_initialized) {

            is_app_initialized = true;
        }

        
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
						break;
					case SDLK_F:
						is_fullscreen = !is_fullscreen;
						SDL_SetWindowFullscreen(window, is_fullscreen);
						private_resize();
						continue;
						break;
					} 
					break;
                case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
                    private_resize();
                    break;
				}
                
				ImGui_ImplSDL3_ProcessEvent(&e);
				if (io.WantCaptureKeyboard || io.WantCaptureMouse) {
					continue;
				}

				// Dispatch other events
				switch (e.type) {
				case SDL_EVENT_KEY_DOWN:

					gz_info("Keydown:{}", SDL_GetKeyName(e.key.key));
					break;
				}
			}

			if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
            {
                SDL_Delay(10);
                continue;
            }

			// No need for checking for resize as it is handled
			private_pre_render();
            private_on_imgui_render();
//			// User defined module tick here
			cr_plugin_update(ctx);
//			// System, module, etc tick once after user update.
//            // Module update: Animation, physics, ai, custom system, etc...
			world.progress(m_frame_data.deltaTime);
//            
            physics_module.simulate(m_frame_data.deltaTime);

            vec3 sphere_pos = physics_module.get_sphere_position();
            vec3 box_pos = physics_module.get_box_position();			
			
			gz_renderer->set_model_matrix(1, glm::translate(mat4(1.0f), sphere_pos));
			gz_renderer->set_model_matrix(2, glm::translate(mat4(1.0f), box_pos));

            private_render();
            private_post_render();
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
			ImGui_ImplVulkan_RemoveTexture((VkDescriptorSet)main_tex_id);
			main_tex_id = (ImTextureID)gz_renderer->get_main_color_texture_imgui_id();
		}
	}

	void App::loop()
	{
        if (!is_app_initialized) {
            is_app_initialized = true;
        }
        
        // Even main_thread_blocked, we still want to run update and render
        // no events
		private_resize();
        private_pre_render();
		world.progress(m_frame_data.deltaTime);
		//            
		physics_module.simulate(m_frame_data.deltaTime);

		vec3 sphere_pos = physics_module.get_sphere_position();
		vec3 box_pos = physics_module.get_box_position();

		gz_renderer->set_model_matrix(1, glm::translate(mat4(1.0f), sphere_pos));
		gz_renderer->set_model_matrix(2, glm::translate(mat4(1.0f), box_pos));

        private_on_imgui_render();
		cr_plugin_update(ctx);
        private_render();
        private_post_render();
	}

	void App::private_pre_render()
	{
		u64 curTime = SDL_GetTicksNS();
        m_frame_data.deltaTime = static_cast<f32>((f64)(curTime - m_frame_data.prevTime) / (f64)SDL_NS_PER_SECOND);
        m_frame_data.prevTime = curTime;
        
        gz_renderer->begin_frame(m_frame_data.deltaTime);
        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

		// put dockspace here so everything is docked
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
	}

	void App::private_post_render()
	{

	}

	void App::private_install_builtin_modules()
	{
		CommonModule common_module;
		common_module.install_into(world, reg);
	}

	void App::private_render()
	{

		// Rendering
        ImGuiIO &io = ImGui::GetIO();
        ImGui::EndFrame();

        ImGui::Render();
        
		ImDrawData* main_draw_data = ImGui::GetDrawData();

		gz_renderer->set_imgui_draw_data(main_draw_data);
		gz_renderer->end_frame();
        
	}

}

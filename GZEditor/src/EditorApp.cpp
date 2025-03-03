#include <GZ_Include.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <imgui.h>
#include <flecs.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include <imgui_stdlib.h>
#include <filesystem>

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


		// Custom ones that are not exported from Imthemes
		style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
		style.ImageBorderSize = 3.0f;
		style.TabBarOverlineSize = 0.0f;
		style.TabBarBorderSize = 1.0f;
	}

	class EditorApp : public GZApp {
	public:
		EditorApp() {
			if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK)) {
				gz_error("SDL init failed: {}", SDL_GetError());
				exit(1);
			}

			auto ent = world.entity("GZEnt");
			gz_info("First entity created using flecs: {}!", ent.name().c_str());
			struct TransformComponent {
				ImVec2 a = ImVec2(1.0f, 0.5f);
				ImVec2 b = ImVec2(0.5f, 1.0f);;
			};

			ent.add<TransformComponent>();
			
			gz_info(ent.get<TransformComponent>()->a.x);

			working_dir = SDL_GetCurrentDirectory();
			
			// TODO(Qiming)(VULKAN)
			// Setup imgui and graphics api...
			// GL 3.0 + GLSL 150
			const char* glsl_version = "#version 150";
            
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
			SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
			
			SDL_WindowFlags window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
			window = SDL_CreateWindow("GZ Editor", 1960, 1080, window_flags);
			if (window == nullptr) {
				gz_error("SDL Create window Failed!: {}", SDL_GetError());
				exit(1);
			}

			SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
			gl_context = SDL_GL_CreateContext(window);
			if (gl_context == nullptr)
			{
				gz_error("SDL Create window Failed!: {}", SDL_GetError());
				exit(1);
			}

			SDL_GL_MakeCurrent(window, gl_context);
			SDL_GL_SetSwapInterval(1); // Enable vsync
			SDL_ShowWindow(window);

			// Imgui setup
			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

			// Filesystem is werid!
			char *wkd = SDL_GetCurrentDirectory();
			gz_info("{} in bytes {}, {}", strlen(wkd), wkd[21], std::string(wkd).size());
            
            #ifdef GZ_PLATFORM_WINDOWS
			std::string path = std::string(wkd) + std::string("asset\\font\\NotoSansTC-Medium.ttf");
            #elif GZ_PLATFORM_APPLE
            std::string path = std::string(wkd) + std::string("asset/font/NotoSansTC-Medium.ttf");
            #endif
			
			SDL_PathInfo info;
			if (!SDL_GetPathInfo(path.c_str(), &info)) {
				gz_warn("{} does not exist!", path);
			}
			gz_info(wkd);
			
			std::filesystem::path p{path};
			if (!std::filesystem::exists(p)) {
				gz_warn("{} does not exist!", path);
			}
			SDL_free(wkd);

			io.Fonts->AddFontFromFileTTF(path.c_str(), 22);
			// Setup Dear ImGui style
			//ImGui::StyleColorsDark();
			//ImGui::StyleColorsLight();
			SetupImGuiStyle();

			// Setup Platform/Renderer backends
			ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
			ImGui_ImplOpenGL3_Init(glsl_version);
		}

		virtual ~EditorApp() override {
			// Cleanup
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplSDL3_Shutdown();
			ImGui::DestroyContext();

			SDL_GL_DestroyContext(gl_context);
			SDL_DestroyWindow(window);
			SDL_Quit();
		}

		virtual void run() override {

			ImGuiIO& io = ImGui::GetIO();
			SDL_Event e;
			while (is_running) {
				
				while (SDL_PollEvent(&e)) {

					switch (e.type) {

					case SDL_EVENT_QUIT:
						gz_info("Quiting...");
						is_running = false;
						break;
					case SDL_EVENT_KEY_DOWN:
						if (e.key.key == SDLK_ESCAPE) {
							is_running = false;
						}
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

				// Start the Dear ImGui frame
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplSDL3_NewFrame();
				ImGui::NewFrame();

				// TODO(Qiming): Dockspace will block viewport clear color?
				// Need to understand viewport and dockspace more!
				ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
				if (show_demo_window)
					ImGui::ShowDemoWindow(&show_demo_window);
				
				{
					static float f = 0.0f;
					static int counter = 0;

					ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

					ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
					ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
					ImGui::Checkbox("Another Window", &show_another_window);

					ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
					ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

					if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
						counter++;
					ImGui::SameLine();
					ImGui::Text("counter = %d", counter);

					ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
					ImGui::End();
				}

				// 3. Show another simple window.
				if (show_another_window)
				{
					ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
					ImGui::Text("Hello from another window!");
					if (ImGui::Button("Close Me"))
						show_another_window = false;
					ImGui::End();
				}

				// Rendering
				ImGui::Render();
				
				glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
				glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
				glClear(GL_COLOR_BUFFER_BIT);
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
				SDL_GL_SwapWindow(window);
			}
			
		}
	private:
		// Most of these setup should be handled by engine
		
		// flecs ecs
		flecs::world world;

		// TODO(Qiming): temp var in editor app
		bool show_demo_window = true;
		bool show_another_window = true;
		ImVec4 clear_color = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
		SDL_GLContext gl_context;
		
		// Asset
		std::string working_dir;
		
		// window
		SDL_Window *window = nullptr;
		bool is_running = true;
	};
}

GZ::GZApp *GZ::create_application() {
	return new EditorApp();
}

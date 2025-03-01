#include <include/GZ_Include.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include <imgui_stdlib.h>

namespace GZ {

	class EditorApp : public GZApp {
	public:
		EditorApp() {
			if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK)) {
				gz_error("SDL init failed: {}", SDL_GetError());
				exit(1);
			}

			// TODO(Qiming)(VULKAN)
			// Setup imgui and graphics api...
			// GL 3.0 + GLSL 130
			const char* glsl_version = "#version 130";
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
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsLight();

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

				if (&show_demo_window)
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
		// TODO(Qiming): temp var in editor app
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
		SDL_GLContext gl_context;
		
		SDL_Window *window = nullptr;
		bool is_running = true;
	};
}

GZ::GZApp *GZ::create_application() {
	return new EditorApp();
}
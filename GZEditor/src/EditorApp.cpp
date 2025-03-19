#include <GZ_Include.h>
#include <defines.h>

#include "EditorApp.h"
#include "CommonModule.h"

namespace ed = ax::NodeEditor;

namespace GZ {

    EditorApp::EditorApp(const AppSpec &spec):
        App(spec)
    {
        // App init is done
        // Editor setup
        
    }
    
    EditorApp::~EditorApp() {
        gz_info("Tearing down Editor App");
    }
    
    void EditorApp::on_init() {
        gz_info("Init Editor App");

		// Node editor
		ed::Config config;
		config.SettingsFile = "Simple.json";
		m_node_Context = ed::CreateEditor(&config);

        gz_info("Init flecs entity", "");
        
        auto ent = world.entity().set<TransformComponent>({vec3(1.0), vec3(1.0), vec3(1.0)});
        
        const TransformComponent *ent_transform = (ent.get<TransformComponent>());
        const TransformComponent t{vec3(1.0f), vec3(2.0f), vec3(1.0f)};
        gz_info("Entity id: {}", ent.id());
        gz_info("Entity transform: {}",  (* ent_transform));
        
    }

    void EditorApp::on_update(const FrameData &frame_data) {
        // actually not much to do here
        
        // just tick system to run or stop the scene
        private_set_frame_data(frame_data);
        
    }

    void EditorApp::on_imgui_render() {
        // render editor stuff
        ImGuiIO& io = ImGui::GetIO();

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

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
			draw_component_imgui_ui_transform(&comp);

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.5f ms/frame (%.2f FPS)", m_cur_frame_data.deltaTime, 1.0f / m_cur_frame_data.deltaTime);
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
            ImGui::SetWindowSize({200.0f, 200.0f});

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


	void EditorApp::private_set_frame_data(const FrameData& frame_data)
	{
        m_cur_frame_data = frame_data;
	}

}


/////////////////////////////////// Entry point function //////////////////////////////////////////
GZ::App *GZ::create_application() {
	AppSpec spec;
	spec.headless = false;
	spec.window_height = 1080;
	spec.window_width = 1960;
    spec.is_fullscreen = false;
	spec.name = "Lost Tower Rogue Like";
	return new EditorApp(spec);
}

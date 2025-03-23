#include <cr.h>
#include <stdio.h>
#include <imgui.h>
#include <Log.h>

#include <CommonModule.h>
#include <App.h>

namespace GZ {
	
    void editor_plugin_load(const EditorContext *data) {
        ImGui::SetCurrentContext(data->imgui_ctx);

        // Trying init some component and system
        
    }

    //static CommonInterface<Component> *comp_interface = new CommonInterface<TransformComponent>();
	void editor_plugin_update(EditorContext *data) {
       
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


		if (data->m_show_node_editor) {
			ImGui::Begin("New editor window");
			ed::SetCurrentEditor(data->m_node_Context);
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


		if (data->m_show_demo_window)
			ImGui::ShowDemoWindow(&data->m_show_demo_window);

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &data->m_show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &data->m_show_another_window);
			ImGui::Checkbox("Node editor", &data->m_show_node_editor);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			if (ImGui::ColorEdit3("clear color", (float*)&data->m_clear_color)) {
				data->gz_renderer->set_clear_value(data->m_clear_color);
			} // Edit 3 floats representing a color

			static TransformComponent comp;
			// Test component draw ui
			/*draw_component_imgui_ui_transform(&comp);*/

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::TextWrapped("Application average %.5f ms/frame (%.2f FPS)", data->frame_data.deltaTime, 1.0f / data->frame_data.deltaTime);
			ImGui::End();
			
		}

		// 3. Show another simple window.
		if (data->m_show_another_window)
		{
			ImGui::Begin("Another Window", &data->m_show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				data->m_show_another_window = false;

			ImGui::End();
		}

		// 4. Show main shaing
		if (data->m_show_main_scene) {

			// rethink ui design later
//            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });

//            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse;
			ImGui::Begin("Main Scene", &data->m_show_main_scene, 0);
			ImGui::SetWindowSize({ 200.0f, 200.0f });

			// Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImVec2 main_scene_cur_window_size = ImGui::GetContentRegionAvail();
			if (data->m_main_view_w != static_cast<u32>(main_scene_cur_window_size.x) || data->m_main_view_h != static_cast<u32>(main_scene_cur_window_size.y)) {
				data->m_main_view_w = static_cast<u32>(main_scene_cur_window_size.x);
				data->m_main_view_h = static_cast<u32>(main_scene_cur_window_size.y);
				data->gz_renderer->set_viewport_size(data->m_main_view_w, data->m_main_view_h);
			}

			ImGui::PushStyleVar(ImGuiStyleVar_ImageBorderSize, 0.0f);
			ImGui::Image((ImTextureID)data->main_tex_id, main_scene_cur_window_size, { 0, 0 }, { 1, 1 });
			ImGui::PopStyleVar();

			ImGui::End();
			//            ImGui::PopStyleVar();
		}

        ImGui::Begin("Another Window1");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        ImGui::Text("Hello from another window second edition!");
        ImGui::Text("Testtest!");
        if (ImGui::Button("Tick Me"))
            gz_info("Oh yeah asd sa!");

        auto e1 = data->world->lookup("Hello");
        
		/*const char* a = GZ_COMPONENT_TYPE_IMPL_DRAW(Hello);
		gz_info(a);*/
        //gz_info(e1.id());

        World *world = data->world;
        ComponentRegistry *reg = data->reg;

        TransformComponent *transform = e1.get_mut<TransformComponent>();
        ImGui::TextColored({0.5, 0.7, 0.5, 1.0}, "Hello");
        ImGui::TextColored({0.5, 0.7, 0.5, 1.0}, "2123");
        //draw_component_imgui_ui_transform(transform);
        DrawComponentContext ctx;
        ctx.name = "position";
        //compInter->draw_imgui(&transform->p, reg, world, &ctx);

		size_t i = 0;
		auto e = world->lookup("Hello");
		std::shared_ptr<IDrawComponentInterfaceName> cur;
		e.each([&](Identifier id) {
			ImGui::PushID(i++);
			void *comp = e.get_mut(id.raw_id());
			cur = reg->get_draw_interface(id);
			if (cur) {
				cur->draw_imgui(comp, reg, world, &ctx);
			}
			else {
				const char * name = id.type_id().name().c_str();
				
				if (name == nullptr) {
					gz_warn("No name on this component!");
					return;
				}
				ImGui::LabelText("", "%s Component is Not Drawable!", name);
			}
			ImGui::PopID();
		});
        
        //comp_interface->draw_second_stuff(transform);
        
        static vec3 color;
        ImGui::ColorEdit3("Some phacy stuff", &color[0]);
        ImGui::TextColored({0.5, 0.7, 0.5, 1.0}, "Yo yo how about some more fancy stuff");
        ImGui::TextColored({0.2, 0.3, 0.1, 1.0}, "Yo yo how about this fancy stuff");
        ImGui::TextColored({0.2, 0.3, 0.1, 1.0}, "Yo yo this fancy stuff");

        ImGui::End();
    }
}

using namespace GZ;

CR_EXPORT int cr_main(struct cr_plugin *ctx, enum cr_op operation) {
    //assert(ctx);

    
    EditorContext *data = (EditorContext *)(ctx->userdata);
    switch (operation) {
        case CR_LOAD:  editor_plugin_load(data); return true; // loading back from a reload

        case CR_STEP:
            editor_plugin_update(data);
            return true;
        case CR_UNLOAD: return true; // preparing to a new reload
        case CR_CLOSE: return true; // the plugin will close and not reload anymore
    }

    
    return true;
}

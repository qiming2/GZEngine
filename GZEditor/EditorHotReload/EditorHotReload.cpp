#include <cr.h>
#include <imgui.h>
#include <SDL3/SDL_mouse.h>

#include "Log.h"
#include "CommonModule.h"
#include "RenderModule.h"
#include "App.h"
#include "MathUtil.h"
#include "Input.h"

namespace GZ {
	
    struct EditorHotReload {
        
    private:
        World *world;
        ComponentRegistry *reg;
        ImGuiContext *imgui_ctx;
        Renderer *gz_renderer;
        Profiler *profiler;
        Input *input;
        ed::EditorContext *node_context = nullptr;
        SDL_Window *window = nullptr;
        ImTextureID main_tex_id;
        
        b8 m_show_demo_window = true;
        b8 m_show_another_window = true;
        b8 m_show_node_editor = true;
        b8 m_show_main_scene = true;
        
        b8 m_show_cursor = true;
        u32 m_main_view_w = 0, m_main_view_h = 0;

        vec4 m_clear_color = vec4(26/255.0f, 26/255.0f, 26/255.0f, 1.00f);
        FrameData frame_data;
        
    public:
        void editor_plugin_load(const EditorContext *data) {
            ImGui::SetCurrentContext(data->imgui_ctx);

            // Trying init some component and system
            world = data->world;
            reg = data->reg;
            imgui_ctx = data->imgui_ctx;
            gz_renderer = data->gz_renderer;
            profiler = data->profiler;
            input = data->input;
            node_context = data->m_node_context;
            window = data->window;
        }
        
        void per_frame_param_update(EditorContext *data) {
            main_tex_id = data->main_tex_id;
            frame_data = data->frame_data;
        }

        //static CommonInterface<Component> *comp_interface = new CommonInterface<TransformComponent>();
        void editor_plugin_update(EditorContext *data) {
            per_frame_param_update(data);
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
                ed::SetCurrentEditor(node_context);
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
                    data->gz_renderer->set_clear_value(m_clear_color);
                } // Edit 3 floats representing a color

                static TransformComponent comp;
                // Test component draw ui
                /*draw_component_imgui_ui_transform(&comp);*/

                if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);
                
                f64 per_frame_ms = data->profiler->get_last_per_frame_data().measured_time / (f64)SDL_NS_PER_MS;
                ImGui::TextWrapped("Application average %.5f ms/frame (%.2f FPS)", per_frame_ms, 1.0f / per_frame_ms * 1000.0f);

                data->profiler->walk_last_perscope_frame_data([&](PerScopeProfilerData scope_profiler_data) {
                    f64 scope_frame_ms = get_ms_from_ns(scope_profiler_data.measured_time);
                    ImGui::TextWrapped("%s %.5f ms/frame1", scope_profiler_data.name.c_str(), scope_frame_ms);
                });
                
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
               
                ImGui::Begin("Main Scene", &m_show_main_scene, 0);
                ImGui::SetWindowSize({ 200.0f, 200.0f });
                static b8 is_main_view_focused = false;
                if (ImGui::IsWindowFocused()) {
                    // Maybe hide cursor?
                    io.WantCaptureKeyboard = false;
                    io.WantCaptureMouse = false;
                    // Show/Hide cursor
                    
                    is_main_view_focused = true;
                } else {
                    is_main_view_focused = false;
                }
                ImVec2 main_scene_cur_window_size = ImGui::GetContentRegionAvail();
                ImGui::PushStyleVar(ImGuiStyleVar_ImageBorderSize, 0.0f);
                ImGui::Image((ImTextureID)main_tex_id, main_scene_cur_window_size, { 0, 0 }, { 1, 1 });
                // Check whether scene is focused whether
                // we are in runtime mode
                
                static f32 move_speed = 2.0f;
                static f32 acceleration = 2.0f;
                // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                ImVec2 main_scene_cur_start_pos = ImGui::GetItemRectMin();
                
                SDL_Rect rect = {.x = static_cast<int>(main_scene_cur_start_pos.x), .y = static_cast<int>(main_scene_cur_start_pos.y), .w = static_cast<int>(main_scene_cur_window_size.x), .h = static_cast<int>(main_scene_cur_window_size.y)};
                data->world->each([&](CameraComponent &cam_comp, TransformComponent &t_comp){
                    if (cam_comp.is_perspective) {
                        if (cam_comp.aspect != static_cast<f32>(main_scene_cur_window_size.x / main_scene_cur_window_size.y)) {
                            cam_comp.fov_y = glm::radians(45.0f);
                            cam_comp.aspect = static_cast<f32>(main_scene_cur_window_size.x / main_scene_cur_window_size.y);
                        }
                    } else {
                        if (cam_comp.viewport_w != main_scene_cur_window_size.x || cam_comp.viewport_h != main_scene_cur_window_size.y) {
                            cam_comp.viewport_w = main_scene_cur_window_size.x;
                            cam_comp.viewport_h = main_scene_cur_window_size.y;
                        }
                    }
                    
                    
                    b8 is_relative_mode = SDL_GetWindowRelativeMouseMode(window);
                    int w, h;
                    SDL_GetWindowSize(window, &w, &h);
                    if (is_main_view_focused) {
                        
                        if (input->is_key_pressed(SCANCODE_H)) {
                            m_show_cursor = !m_show_cursor;
                            gz_info("Change cursor visibility");
                        }
                        
                        if (!m_show_cursor && !is_relative_mode) {
                            SDL_WarpMouseInWindow(window, rect.x + rect.w / 2.0f, rect.y + rect.h / 2.0f);
                            SDL_SetWindowRelativeMouseMode(window, !m_show_cursor);
                            SDL_SetWindowMouseRect(window, &rect);
                            gz_info("Hiding cursor");
                        } else if (m_show_cursor && is_relative_mode) {
                            SDL_WarpMouseInWindow(window, rect.x + rect.w / 2.0f, rect.y + rect.h / 2.0f);
                            SDL_SetWindowRelativeMouseMode(window, !m_show_cursor);
                            SDL_SetWindowMouseRect(window, NULL);
                            gz_info("Show cursor");
                        }
                        
                        mat4 orientation = glm::mat4_cast(t_comp.r);
                        vec3 right = glm::normalize(orientation[0].xyz());
                        vec3 up = glm::normalize(orientation[1].xyz());
                        vec3 forward = -glm::normalize(orientation[2].xyz());
                        if (data->input->is_key_down(SCANCODE_W)) {
                            t_comp.p += forward * data->frame_data.deltaTime * move_speed;
                        }
                        
                        if (data->input->is_key_down(SCANCODE_S)) {
                            t_comp.p -= forward * data->frame_data.deltaTime * move_speed;
                        }

                        if (data->input->is_key_down(SCANCODE_A)) {
                            t_comp.p -= right * data->frame_data.deltaTime * move_speed;
                        }
                        
                        if (data->input->is_key_down(SCANCODE_D)) {
                            t_comp.p += right * data->frame_data.deltaTime * move_speed;
                        }
                        
                        if (data->input->is_key_down(SCANCODE_Q)) {
                            t_comp.p -= up * data->frame_data.deltaTime * move_speed;
                        }
                        
                        if (data->input->is_key_down(SCANCODE_E)) {
                            t_comp.p += up * data->frame_data.deltaTime * move_speed;
                        }

                        
                        f32 y_wheel_delta = data->input->get_mouse_wheel_y_delta();
                        vec2 m_p = data->input->get_mouse_pos();
                        vec2 m_p_delta = data->input->get_mouse_pos_delta();

                        if (glm::abs(y_wheel_delta) > glm::epsilon<f32>()) {
                            move_speed += acceleration * y_wheel_delta * data->frame_data.deltaTime;
                        }

                        if (input->is_key_down(SCANCODE_LCTRL) && input->is_key_pressed(SCANCODE_EQUALS)) {
                            move_speed += 1.5f;
                        }
                        else if (input->is_key_down(SCANCODE_LCTRL) && input->is_key_pressed(SCANCODE_MINUS)) {
                            move_speed -= 1.5f;
                        }
                        move_speed = glm::clamp(move_speed, 2.0f, 10.0f);
                        
                        // Camera rotation
                        quat yaw_delta = GZ_QUAT_IDENTITY;
                        if (glm::abs(m_p_delta.x) > glm::epsilon<f32>()) {
                            // yaw
                            yaw_delta = glm::angleAxis(glm::radians(-m_p_delta.x * frame_data.deltaTime * 10.0f), GZ_UP);
                        }

                        static f32 cam_r_factor = 0.40f;
                        // Can also use up and down arrow
                        if (input->is_key_down(SCANCODE_RIGHT)) {
                            yaw_delta *= glm::angleAxis(GZ_PI * frame_data.deltaTime * cam_r_factor, -GZ_UP);
                        }
                        else if (input->is_key_down(SCANCODE_LEFT)) {
                            yaw_delta *= glm::angleAxis(GZ_PI * frame_data.deltaTime * cam_r_factor, GZ_UP);
                        }
                        
                        quat pitch_delta = GZ_QUAT_IDENTITY;
                        if (glm::abs(m_p_delta.y) > glm::epsilon<f32>()) {
                            // pitch
                            pitch_delta = glm::angleAxis(glm::radians(-m_p_delta.y * frame_data.deltaTime * 10.0f), GZ_RIGHT);
                        }

						// Can also use up and down arrow
						if (input->is_key_down(SCANCODE_UP)) {
							pitch_delta *= glm::angleAxis(glm::radians(GZ_PI * frame_data.deltaTime * 10.0f), GZ_RIGHT);
						}
						else if (input->is_key_down(SCANCODE_DOWN)) {
							pitch_delta *= glm::angleAxis(glm::radians(GZ_PI * frame_data.deltaTime * 10.0f), -GZ_RIGHT);
						}
                        
                        t_comp.r = glm::normalize(yaw_delta * t_comp.r * pitch_delta);

                    } else {
                        if (is_relative_mode) {
                            SDL_WarpMouseInWindow(window, rect.x + rect.w / 2.0f, rect.y + rect.h / 2.0f);
                            SDL_SetWindowRelativeMouseMode(window, false);
                            SDL_SetWindowMouseRect(window, NULL);
                        }
                    }
                    
                });

                ImGui::PopStyleVar();
                ImGui::End();
            }

            ImGui::Begin("Another Window1");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello yeeeee from another window!");
            ImGui::Text("Hello from another window second edition!");
            ImGui::Text("Testtest!");
            if (ImGui::Button("Tick Me"))
                gz_info("Oh yeah asd sa!");
     
            World *world = data->world;
            ComponentRegistry *reg = data->reg;

            DrawComponentContext ctx;
            ctx.name = "position";
            //compInter->draw_imgui(&transform->p, reg, world, &ctx);

            size_t i = 0;
            auto e = world->lookup("Player");
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
            ImGui::ColorEdit3("Good stuff", &color[0]);
            ImGui::TextColored({0.5, 0.7, 0.5, 1.0}, "Yo yo how about some more fancy stuff");
            ImGui::TextColored({0.4, 0.3, 0.1, 1.0}, "Yo yo how about different color?");
            ImGui::TextColored({0.2, 0.3, 0.1, 1.0}, "Yo yo this fancy stuff");
            static b8 yaxi = false;
            ImGui::Checkbox("Xiaoggasdasdasen ya xi!", &yaxi);
            if (yaxi) {
                ImGui::TextColored({0.2, 0.3, 0.1, 1.0}, "Ya xi la!");
            }
            ImGui::End();
        }
    };
}
    

using namespace GZ;

CR_EXPORT int cr_main(struct cr_plugin *ctx, enum cr_op operation) {
    static EditorHotReload hot_reload;
    EditorContext *data = (EditorContext *)(ctx->userdata);
    switch (operation) {
        case CR_LOAD:
            hot_reload.editor_plugin_load(data);
            return true; // loading back from a reload

        case CR_STEP:
            hot_reload.editor_plugin_update(data);
            return true;
        case CR_UNLOAD:
            return true; // preparing to a new reload
        case CR_CLOSE:
            return true; // the plugin will close and not reload anymore
    }

    
    return true;
}

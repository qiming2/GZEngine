#include <cr.h>
#include <imgui.h>
#include <SDL3/SDL_mouse.h>

#include "Log.h"
#include "CommonModule.h"
#include "RenderModule.h"
#include "CharacterModule.h"
#include "App.h"
#include "MathUtil.h"
#include "Input.h"
#include "flecs.h"

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

        PhysicsModule *m_physics_module = nullptr;
		RenderModule* m_render_module = nullptr;
        
        b8 m_show_demo_window = true;
        b8 m_show_another_window = true;
        b8 m_show_node_editor = true;
        b8 m_show_main_scene = true;
        
        b8 m_show_cursor = true;
        u32 m_main_view_w = 0, m_main_view_h = 0;

        vec4 m_clear_color = vec4(26/255.0f, 26/255.0f, 26/255.0f, 1.00f);
        FrameData frame_data;
        
        // Camera Stuff
		size_t m_primary_cam_index = 0;
		size_t m_num_total_cams = 1;
        flecs::query<CameraComponent> cam_q;
        flecs::query<CameraComponent, TransformComponent> cam_trans_q;

        ModuleContext *module_ctx;
        // Player
        Entity player_ent;

        // Scene Specific Data
        SceneModule *m_scene_module;
        TransformModule *m_transform_module;
        Entity m_scene_root;
    public:
        void editor_plugin_unload(const EditorContext *data) {
            cam_q.destruct();
            cam_trans_q.destruct();
        }
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
            
            module_ctx = data->module_ctx;

			m_physics_module = data->module_ctx->module_reg->get_module_by_name<PhysicsModule>();
			m_render_module = data->module_ctx->module_reg->get_module_by_name<RenderModule>();
            m_scene_module = data->module_ctx->module_reg->get_module_by_name<SceneModule>(); 
            m_transform_module = data->module_ctx->module_reg->get_module_by_name<TransformModule>();

            cam_q = world->query<CameraComponent>("cam_q");
            cam_trans_q = world->query<CameraComponent, TransformComponent>("cam_trans_q");
            m_num_total_cams = 0;
            m_primary_cam_index = 0;
            b8 has_primary_cam = false;
            // Only Select the first primary camera for rendering, toggle off the other cams
            // who are set up as primary cam
            cam_q.each([&](Entity ent, CameraComponent& cam_comp) {
                gz_info("Camera: {}", ent.name().c_str());
                m_num_total_cams++;
                if (!cam_comp.is_primary) return;

                if (!has_primary_cam) {
                    has_primary_cam = true;
                    m_primary_cam_index = m_num_total_cams-1;
                }
                else {
                    cam_comp.is_primary = false;
                }
            });

            if (!m_num_total_cams) gz_warn("There is no camera in the scene!");
            
            player_ent = m_scene_module->lookup("Player");
            if (!player_ent.is_valid()) gz_warn("Player Entity is not alive in the scene?");

            m_scene_root = m_scene_module->get_scene_root_entity();
        }
        
        void private_per_frame_param_update(EditorContext *data) {
            main_tex_id = data->main_tex_id;
            frame_data = data->frame_data;
        }

        //static CommonInterface<Component> *comp_interface = new CommonInterface<TransformComponent>();
        void editor_plugin_update(EditorContext *data) {
            private_per_frame_param_update(data);
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

                ImGui::Begin("Simple Config");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window", &m_show_demo_window);      // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &m_show_another_window);
                ImGui::Checkbox("Node editor", &m_show_node_editor);

                ImGui::End();
                
            }

            {
				ImGui::Begin("Stats Panel");

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
                ImGui::Begin("Module window", &m_show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
				if (ImGui::ColorEdit3("clear color", (float*)&m_clear_color)) {
					data->gz_renderer->set_clear_value(m_clear_color);
				} // Edit 3 floats representing a color
                ImGui::Text("This window is for exposing module settings");
                ImGui::Checkbox("Debug Physics", &m_physics_module->is_physics_debug_on);
                if (ImGui::Button("Clear Scene")) {
                    m_scene_module->clear_scene();
                }
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
                
                // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                ImVec2 main_scene_cur_start_pos = ImGui::GetItemRectMin();
                
                SDL_Rect rect = {.x = static_cast<int>(main_scene_cur_start_pos.x), .y = static_cast<int>(main_scene_cur_start_pos.y), .w = static_cast<int>(main_scene_cur_window_size.x), .h = static_cast<int>(main_scene_cur_window_size.y)};

                if (is_main_view_focused && input->is_key_pressed(SCANCODE_TAB)) {
                    // Switch camera here
                    m_primary_cam_index = (m_primary_cam_index + 1) % m_num_total_cams;
                    cam_q.each([&](WorldIter &it, size_t index, CameraComponent &cam_comp) {
                        if (index == m_primary_cam_index) {
                            cam_comp.is_primary = true;
                        }
                        else {
                            cam_comp.is_primary = false;
                        }
                    });
                }

                world->defer([&] {
                    cam_trans_q.each([&](Entity ent, CameraComponent &cam_comp, TransformComponent &t_comp){
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

                        if (!cam_comp.is_primary) return;
                        // Switch cam mode
                        switch (m_primary_cam_index) {
                        case 0:
                            if (!input->is_key_down(SCANCODE_LALT)) {
                                return;
                            }
                            private_flying_cam_controller(ent, t_comp);
                            break;
                        case 1:
                            //private_follow_cam_controller(t_comp);
                            break;
                        }
                    });
                });
                
                
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
                    

                } else {
                    if (is_relative_mode) {
                        SDL_WarpMouseInWindow(window, rect.x + rect.w / 2.0f, rect.y + rect.h / 2.0f);
                        SDL_SetWindowRelativeMouseMode(window, false);
                        SDL_SetWindowMouseRect(window, NULL);
                    }
                }
                ImGui::PopStyleVar();
                ImGui::End();
            }

            // Entity Selection Context
            static Entity selected_ent = Entity();
			{
				ImGui::Begin("Entity Tree");
                ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.0f, 0.3333333402872086f, 0.4490196138620377f, 1.0f));

				// 'selection_mask' is dumb representation of what may be user-side selection state.
				//  You may retain selection state inside or outside your objects in whatever format you see fit.
				// 'node_clicked' is temporary storage of what node we have clicked to process selection at the end
				/// of the loop. May be a pointer to your own node type, etc.
                static int selection_mask = 0;
				
                //Entity current_scene_prefab = m_scene_module->get_active_scene_prefab();
                Entity current_scene = m_scene_module->get_active_scene();
				const TagComponent* tag_scene = current_scene.get<TagComponent>();
				const char* scene_name = tag_scene ? tag_scene->name.data() : "Unknown Scene";
				if (ImGui::TreeNodeEx(scene_name, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow))
				{
					static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
					static bool test_drag_and_drop = true;

					if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
						selected_ent = current_scene;
					if (test_drag_and_drop && ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
						ImGui::Text("This is a drag and drop source");
						ImGui::EndDragDropSource();
					}

                    std::function<void(Entity)> draw_entity_tree = [&](Entity ent) {
                        
                        ent.children([&](Entity child){

							ImGuiTreeNodeFlags node_flags = base_flags;
							const bool is_selected = selected_ent == child;
							if (is_selected)
								node_flags |= ImGuiTreeNodeFlags_Selected;

							b8 has_child = world->query_builder()
								.with(EcsChildOf, child)
								.build().is_true();

                            const TagComponent *tag = child.get<TagComponent>();
                            const char *entity_name = tag ? tag->name.data() : "Unknown Entity";
                            if (has_child) {
								bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)child.id(), node_flags, " %s", entity_name);
								if (ImGui::IsItemClicked() || ImGui::IsItemClicked(1) && !ImGui::IsItemToggledOpen())
									selected_ent = child;
								if (test_drag_and_drop && ImGui::BeginDragDropSource())
								{
									ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
									ImGui::Text("This is a drag and drop source");
									ImGui::EndDragDropSource();
								}

								if (node_open)
								{
									draw_entity_tree(child);
									ImGui::TreePop();
								}
                            }
                            else {
                                node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
                                ImGui::TreeNodeEx((void*)(intptr_t)child.id(), node_flags, " %s", entity_name);
								if (ImGui::IsItemClicked() || ImGui::IsItemClicked(1) && !ImGui::IsItemToggledOpen())
									selected_ent = child;
								if (test_drag_and_drop && ImGui::BeginDragDropSource())
								{
									ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
									ImGui::Text("This is a drag and drop source");
									ImGui::EndDragDropSource();
								}
                            }

                        });
                        
                    };
                    /*if (current_scene_prefab)
					    draw_entity_tree(current_scene_prefab);*/
                    if (current_scene)
                        draw_entity_tree(current_scene);

                    ImGui::TreePop();
				}
                ImGui::PopStyleColor();

				if (ImGui::BeginPopupContextWindow("Entity Menus"))
				{
					if (ImGui::MenuItem("Add Entity")) {
						
                        if (selected_ent) {
                            m_scene_module->entity(selected_ent);
                        }
                        else {
							m_scene_module->entity();
                        }
                    }
					if (ImGui::MenuItem("Serialize Entity Tree")) {
						ecs_entity_to_json_desc_t desc;
                        desc.serialize_values = true;
                        desc.serialize_full_paths = true;
                        if (selected_ent) {
                            gz_info("{}", selected_ent.to_json(&desc));
                        }
                        else {
                            //selected_ent.from_json();
                        }

					}
					ImGui::EndPopup();
				}
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered() && ImGui::IsWindowHovered()) {
                    selected_ent = Entity();
                }
                
                
				ImGui::End();
			}

			{
				ImGui::Begin("Properties Panel");

				size_t component_count = 0;
				std::shared_ptr<IDrawComponentInterfaceName> cur;
				static constexpr std::string_view component_str = " Component";
				if (selected_ent.is_valid() && selected_ent != m_scene_root) {
					ComponentRegistry* reg = data->reg;
					DrawComponentContext ctx;
					ctx.name = "TransformComponent";
                    ctx.e = selected_ent;
                    ImGui::Text("id: %llu", selected_ent.id());
					selected_ent.each([&](Identifier id) {
						ImGui::PushID(component_count++);
						void* comp = selected_ent.get_mut(id.raw_id());
						cur = reg->get_draw_interface(id);
                        
						if (cur) {
							cur->draw_imgui(comp, module_ctx, &ctx);
						}
						else {
							std::string name("");

                            // Only draw component
							if (id.is_entity()) {
								name = id.entity().name().c_str();
								if (name == "") {
									gz_warn("No name on this component!");
									return;
								}
								ImGui::LabelText("", "%s is Not Drawable!", name.c_str());
							}
						}
						ImGui::PopID();
					});
                }
                else {
                    ImGui::TextColored({0, 1, 0, 1}, "Select Something...");
                }

				ImGui::End();
			}
        }

        void private_follow_cam_controller(TransformComponent& t_comp) {
            gz_info("Follow player");
            const TransformComponent *player_t_comp = player_ent.get<TransformComponent>();

			static vec3 cam_offset = {4, 8, 4};
            vec3 look_dir = glm::normalize(- cam_offset);
            t_comp.p = player_t_comp->p + cam_offset;
            t_comp.r = glm::quatLookAt(look_dir, GZ_UP);
        }

        void private_flying_cam_controller(Entity ent, TransformComponent &t_comp) {
            
            TransformComponent world_t_comp = m_transform_module->world_transform_component(ent);
			static f32 move_speed = 2.0f;
			static f32 acceleration = 2.0f;

			mat4 orientation = glm::mat3_cast(t_comp.r);
			vec3 right = glm::normalize(orientation[0].xyz());
			vec3 up = glm::normalize(orientation[1].xyz());
			vec3 forward = -glm::normalize(orientation[2].xyz());
			if (input->is_key_down(SCANCODE_W)) {
                world_t_comp.p += forward * frame_data.deltaTime * move_speed;
			}

			if (input->is_key_down(SCANCODE_S)) {
                world_t_comp.p -= forward * frame_data.deltaTime * move_speed;
			}

			if (input->is_key_down(SCANCODE_A)) {
                world_t_comp.p -= right * frame_data.deltaTime * move_speed;
			}

			if (input->is_key_down(SCANCODE_D)) {
                world_t_comp.p += right * frame_data.deltaTime * move_speed;
			}

			if (input->is_key_down(SCANCODE_Q)) {
                world_t_comp.p -= up * frame_data.deltaTime * move_speed;
			}

			if (input->is_key_down(SCANCODE_E)) {
                world_t_comp.p += up * frame_data.deltaTime * move_speed;
			}


			f32 y_wheel_delta = input->get_mouse_wheel_y_delta();
			vec2 m_p = input->get_mouse_pos();
			vec2 m_p_delta = input->get_mouse_pos_delta();

			if (glm::abs(y_wheel_delta) > glm::epsilon<f32>()) {
				move_speed += acceleration * y_wheel_delta * frame_data.deltaTime;
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

            world_t_comp.r = glm::normalize(yaw_delta * world_t_comp.r * pitch_delta);
            t_comp.from_mat4(glm::inverse(m_transform_module->world_transform(ent.parent())) * world_t_comp.get_mat4());

            ent.modified<TransformComponent>();
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
            hot_reload.editor_plugin_unload(data);
            return true; // preparing to a new reload
        case CR_CLOSE:
            hot_reload.editor_plugin_unload(data);
            return true; // the plugin will close and not reload anymore
    }

    
    return true;
}

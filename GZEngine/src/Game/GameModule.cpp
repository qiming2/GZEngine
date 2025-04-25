#include <gzpch.h>

#include "GameModule.h"
#include "PhysicsModule.h"
#include "RenderModule.h"
#include "MathUtil.h"

namespace GZ {

    

    void GameModule::install_into(const ModuleContext& module_ctx) {
        
		m_scene_module = module_ctx.module_reg->get_module<SceneModule>();
		m_transform_module = module_ctx.module_reg->get_module<TransformModule>();

		static f32 rotate_scale = 2.0f;
		static f32 move_speed = 15.0f;
		static Input *m_input = module_ctx.input;
		static World &world = *module_ctx.world;
		world.system("Character Controller")
			.write<CharacterComponent, TransformComponent>()
			.run([&](WorldIter& it) {
			Entity player = m_scene_module->lookup("Player");
			TransformComponent* t = player.get_mut<TransformComponent>();
			CharacterComponent* char_comp = player.get_mut<CharacterComponent>();

			Entity follow_cam = m_scene_module->lookup("Character Camera");
			const TransformComponent* cam_t_comp = follow_cam.get<TransformComponent>();
			mat3 orientation = mat3_cast(cam_t_comp->r);
			vec3 right = glm::normalize(vec3{ orientation[0].x, 0, orientation[0].z });
			vec3 up = GZ_UP;
			vec3 forward = -glm::normalize(glm::cross(right, up));

			vec2 move_axis = { 0, 0 };
			b8 is_moving = false;
			if (m_input->is_key_down(SCANCODE_W)) {
				move_axis.y = 1.0f;
				is_moving = true;
			}
			else if (m_input->is_key_down(SCANCODE_S)) {
				move_axis.y = -1.0f;
				is_moving = true;
			}

			if (m_input->is_key_down(SCANCODE_D)) {
				move_axis.x = 1.0f;
				is_moving = true;
			}
			else if (m_input->is_key_down(SCANCODE_A)) {
				move_axis.x = -1.0f;
				is_moving = true;
			}

			if (is_moving) {
				char_comp->vel = glm::normalize(move_axis.x * right + move_axis.y * forward) * move_speed;

				vec3 new_forward = glm::normalize(move_axis.x * right + move_axis.y * forward);
				t->r = glm::quatLookAt(-new_forward, GZ_UP);
			}
			else {
				char_comp->vel = vec3(0, 0, 0);
			}

		});

		static vec3 cam_dir = -glm::normalize(vec3{ 1, 1, 1 });
		static f32 cam_dist_min = 2.0f;
		static f32 cam_dist_max = 10.0f;
		static f32 zoom_speed = 400.0f;
		static f32 cur_cam_dist = 3.0f;
		static f32 cam_pitch = 45.0f; // in degrees
		static f32 cam_yaw = 45.0f; // in degrees
		static f32 change_look_dir_speed = 50.0f;
		world.system("Following Camera")
			.kind(flecs::PostUpdate)
			.write<CameraComponent, TransformComponent>()
			.run([=](WorldIter& it) {
			Entity follow_cam = m_scene_module->lookup("Character Camera");
			Entity player = m_scene_module->lookup("Player");
			const CameraComponent* cam_comp = follow_cam.get<CameraComponent>();
			if (!cam_comp || !cam_comp->is_primary) return;
			TransformComponent* player_t_comp = player.get_mut<TransformComponent>();
			TransformComponent* cam_t_comp = follow_cam.get_mut<TransformComponent>();

			// Instead of directly using TransformComponent, we should interpolate
			// so to reduce stutter, we should do this in physics module

			f32 y_delta = m_input->get_mouse_wheel_y_delta();
			if (m_input->is_mouse_wheel_changed()) {
				cur_cam_dist = glm::clamp(cur_cam_dist - y_delta * zoom_speed * it.delta_time(), cam_dist_min, cam_dist_max);
			}

			if (m_input->is_key_down(SCANCODE_RIGHT)) {
				cam_yaw += it.delta_time() * change_look_dir_speed;
			}
			else if (m_input->is_key_down(SCANCODE_LEFT)) {
				cam_yaw -= it.delta_time() * change_look_dir_speed;
			}

			// Can also use up and down arrow
			if (m_input->is_key_down(SCANCODE_UP)) {
				cam_pitch += it.delta_time() * change_look_dir_speed;
			}
			else if (m_input->is_key_down(SCANCODE_DOWN)) {
				cam_pitch -= it.delta_time() * change_look_dir_speed;
			}

			cam_pitch = glm::clamp(cam_pitch, 1.0f, 89.0f);
			cam_yaw = std::fmodf(cam_yaw, 360.0f);
			f32 pitch_radians = glm::radians(cam_pitch);
			f32 yaw_radians = glm::radians(cam_yaw);
			vec3 look_dir = glm::normalize(-vec3{ glm::cos(yaw_radians) * glm::cos(pitch_radians), glm::sin(pitch_radians), glm::sin(yaw_radians) * glm::cos(pitch_radians) });
			cam_dir = look_dir;
			cam_t_comp->p = player_t_comp->p - cam_dir * cur_cam_dist;
			cam_t_comp->r = glm::quatLookAt(look_dir, GZ_UP);
		});
    }

    void GameModule::uninstall_from(const ModuleContext& ctx) {
        
    }

	void GameModule::create_default_game_objects()
	{
		// Register scenes/prefabs/objects

		
	}

}

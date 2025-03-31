#pragma once

#include <array>
#include <SDL3/SDL.h>
#include <defines.h>
#include "Scancode.h"

// I don't think there would be 50 INPUT events happening in a frame for any game
#define MAX_INPUT_EVENT 50

namespace GZ {
    struct Input {
    public:
        static Input g_input_instance;
        static void init();
        
    public:
        // This needs to be called after polling sdl events
        void begin_frame();
        void add_input_event(SDL_Event& e);
        GZ_API b8 is_key_pressed(Scancode code);
        GZ_API b8 is_key_released(Scancode code);
        GZ_API b8 is_key_down(Scancode code);
        GZ_API b8 is_key_up(Scancode code);
        GZ_API b8 is_action_pressed(const std::string &action_name);
        GZ_API vec2 get_mouse_pos_delta();
        GZ_API vec2 get_mouse_pos();
        GZ_API vec2 get_mouse_wheel_delta();
        GZ_API f32 get_mouse_wheel_y_delta();
        GZ_API void set_action_keycode(const std::string& action_name); // haven't decided yet
        // void is_action_released(); // Don't really think it is needed here
    private:

        struct InputState {
            // Can have more states later like joystick
            std::array<b8, SCANCODE_COUNT> key_states;

            // Mouse
			vec2 mouse_p;
			vec2 mouse_p_delta;
			vec2 mouse_wheel_delta;
        };
        
    private: // Internal structure
        // double buffering states so we can determine
        // whether keys are pressed or released
        std::array<InputState, 2> m_input_states;
        std::array<SDL_Event, 50> m_input_events;
        size_t m_event_count = 0;
    };

}

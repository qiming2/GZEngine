#pragma once

#include <array>
#include <SDL3/SDL.h>
#include <defines.h>
#include "Scancode.h"

namespace GZ {
    struct Input {
    public:
        static Input g_input_instance;
        static void init();
    public:
        // This needs to be called after polling sdl events
        void begin_frame();
        b8 is_key_pressed(Scancode code);
        b8 is_key_released(Scancode code);
        b8 is_key_down(Scancode code);
        b8 is_key_up(Scancode code);
        b8 is_action_pressed(const std::string &action_name);
        void set_action_keycode(const std::string &action_name); // haven't decided yet
        // void is_action_released(); // Don't really think it is needed here
    private:
        struct InputState {
            // Can have more states later like joystick
            std::array<b8, SCANCODE_COUNT> key_states;
        };
        
    private: // Internal structure
        // double buffering states so we can determine
        // whether keys are pressed or released
        std::array<InputState, 2> input_states;
    };

}

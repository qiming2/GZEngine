#include <gzpch.h>

#include "Input.h"

namespace GZ {
Input Input::g_input_instance;

void Input::init() {
    Input::g_input_instance = Input();
    // Get a copy of the initial states from sdl
    const b8 *keystates = SDL_GetKeyboardState(NULL);
    
    for (size_t i = 0; i < SCANCODE_COUNT; ++i) {
        g_input_instance.input_states[0].key_states[i] = keystates[0];
    }

    
}

void Input::begin_frame() {

    std::swap(g_input_instance.input_states[0], g_input_instance.input_states[1]);
    
    const b8 *keystates = SDL_GetKeyboardState(NULL);
    
    for (size_t i = 0; i < SCANCODE_COUNT; ++i) {
        g_input_instance.input_states[0].key_states[i] = keystates[i];
    }
}

b8 Input::is_key_pressed(Scancode code) {
    return !g_input_instance.input_states[1].key_states[code] && g_input_instance.input_states[0].key_states[code];
}
b8 Input::is_key_released(Scancode code) {
    return g_input_instance.input_states[1].key_states[code] && !g_input_instance.input_states[0].key_states[code];
}
b8 Input::is_key_down(Scancode code) {
    // Just need to check current key state
    return g_input_instance.input_states[0].key_states[code];
}
b8 Input::is_key_up(Scancode code) {
    return !g_input_instance.input_states[0].key_states[code];
}
b8 Input::is_action_pressed(const std::string &action_name) {
    
}
void Input::set_action_keycode(const std::string &action_name) {
    
}
}

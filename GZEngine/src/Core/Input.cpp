#include <gzpch.h>

#include "Input.h"

namespace GZ {
Input Input::g_input_instance;

void Input::init() {
    Input::g_input_instance = Input();
    InputState &first_state = g_input_instance.m_input_states[0];
    // Get a copy of the initial states from sdl

    // Keyboard
    const b8 *keystates = SDL_GetKeyboardState(NULL);
    
    for (size_t i = 0; i < SCANCODE_COUNT; ++i) {
        first_state.key_states[i] = keystates[0];
    }

    // Mouse
    SDL_GetMouseState(&first_state.mouse_p.x, &first_state.mouse_p.y);
    first_state.mouse_p_delta = vec2{0.0f, 0.0f};
    first_state.mouse_wheel_delta = vec2{0.0f, 0.0f};
    
    g_input_instance.m_event_count = 0;
    
}

void Input::add_input_event(SDL_Event &e) {
    m_input_events[m_event_count++] = e;
}

void Input::begin_frame() {
    std::swap(g_input_instance.m_input_states[0], g_input_instance.m_input_states[1]);
    
    const b8 *keystates = SDL_GetKeyboardState(NULL);
    
    for (size_t i = 0; i < SCANCODE_COUNT; ++i) {
        g_input_instance.m_input_states[0].key_states[i] = keystates[i];
    }

    // we process any event
    for (size_t i = 0; i < m_event_count; ++i) {
        SDL_Event &e = m_input_events[i];
        switch (e.type) {
        // only care about y axis mouse wheel for now
        case SDL_EVENT_MOUSE_WHEEL:
            SDL_MouseWheelEvent &wheel_e = e.wheel;
            vec2 wheel_delta = vec2{wheel_e.x, wheel_e.y};
            if (wheel_e.direction == SDL_MOUSEWHEEL_FLIPPED) {
                wheel_delta = -wheel_delta;
            }

            g_input_instance.m_input_states[0].mouse_wheel_delta = wheel_delta;
            continue;
            break;
        }
    }
    vec2 cur_mouse_pos;
    const vec2 &pre_mouse_pos = g_input_instance.m_input_states[1].mouse_p;
    SDL_GetMouseState(&cur_mouse_pos.x, &cur_mouse_pos.y);
    SDL_GetRelativeMouseState(&g_input_instance.m_input_states[0].mouse_p_delta.x, &g_input_instance.m_input_states[0].mouse_p_delta.y);
    g_input_instance.m_input_states[0].mouse_p = cur_mouse_pos;
    m_event_count = 0;
}

b8 Input::is_key_pressed(Scancode code) {
    return !g_input_instance.m_input_states[1].key_states[code] && g_input_instance.m_input_states[0].key_states[code];
}
b8 Input::is_key_released(Scancode code) {
    return g_input_instance.m_input_states[1].key_states[code] && !g_input_instance.m_input_states[0].key_states[code];
}
b8 Input::is_key_down(Scancode code) {
    // Just need to check current key state
    return g_input_instance.m_input_states[0].key_states[code];
}
b8 Input::is_key_up(Scancode code) {
    return !g_input_instance.m_input_states[0].key_states[code];
}
b8 Input::is_action_pressed(const std::string &action_name) {
    return true;
}

vec2 Input::get_mouse_pos_delta() {
    return g_input_instance.m_input_states[0].mouse_p_delta;
}

vec2 Input::get_mouse_pos() {
    return g_input_instance.m_input_states[0].mouse_p;
}

vec2 Input::get_mouse_wheel_delta() {
    return g_input_instance.m_input_states[0].mouse_wheel_delta;
}

f32 Input::get_mouse_wheel_y_delta() {
    return g_input_instance.m_input_states[0].mouse_wheel_delta.y;
}

void Input::set_action_keycode(const std::string &action_name) {
    
}
}

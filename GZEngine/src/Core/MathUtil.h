#pragma once
#include <SDL3/SDL.h>

#define EPSILON
namespace GZ {

    constexpr f32 GZ_PI = glm::pi<f32>();
    constexpr f32 GZ_TWO_PI = glm::two_pi<f32>();
    constexpr quat GZ_QUAT_IDENTITY = glm::identity<quat>();

    constexpr vec3 GZ_UP = vec3(0.0f, 1.0f, 0.0f);
    constexpr vec3 GZ_RIGHT = vec3(1.0f, 0.0f, 0.0f);
    constexpr vec3 GZ_FORWARD = vec3(0.0f, 0.0f, 1.0f);

	GZ_FORCE_INLINE f64 get_s_from_ns(u64 ns) {
		return static_cast<f64>(ns) / SDL_NS_PER_SECOND;
	}

	GZ_FORCE_INLINE f64 get_ms_from_ns(u64 ns) {
		return static_cast<f64>(ns) / SDL_NS_PER_MS;
	}

	GZ_FORCE_INLINE f64 get_s_from_ms(u64 ms) {
		return static_cast<f64>(ms) / SDL_MS_PER_SECOND;
	}

    constexpr f32 f_epsilon = std::numeric_limits<f32>::epsilon();
    constexpr f32 scale_factor = 1;
    constexpr f32 tolerance_factor = 2;
    GZ_FORCE_INLINE b8 is_approximately_equal(float a, float b) {
//        gz_info("f_epsilon: {}", f_epsilon);
        double scale = std::fmax(abs(a), abs(b));
        return abs(a - b) <= scale_factor * (tolerance_factor * f_epsilon);
    }

    GZ_FORCE_INLINE b8 is_approximately_zero(float val) {
//        gz_info("f_epsilon: {}", f_epsilon);
        return abs(val) <= scale_factor * (tolerance_factor * f_epsilon);
    }

    // no normalization
    // we are doing everything in degrees, so convert to radians
    GZ_FORCE_INLINE quat to_quat(const vec3 &euler_angles) {
        return quat(glm::radians((euler_angles)));
    }

    GZ_FORCE_INLINE vec3 to_euler(const quat& normalized_qua) {
        return glm::degrees(glm::eulerAngles(normalized_qua));
    }
}

#pragma once
#include <SDL3/SDL.h>

#define EPSILON
namespace GZ {

    constexpr f32 GZ_PI = glm::pi<f32>();
    constexpr f32 GZ_TWO_PI = glm::two_pi<f32>();
    constexpr quat GZ_QUAT_IDENTITY = glm::identity<quat>();
    constexpr mat4 GZ_MAT4_IDENTITY = glm::identity<mat4>();
    constexpr vec3 GZ_TRANSLATION_ZERO = vec3(0.0, 0.0, 0.0);
    constexpr vec3 GZ_SCALE_ZERO = vec3(0.0, 0.0, 0.0);

    constexpr vec3 GZ_UP = vec3(0.0, 1.0, 0.0);
    constexpr vec3 GZ_RIGHT = vec3(1.0, 0.0, 0.0);
    constexpr vec3 GZ_FORWARD = vec3(0.0, 0.0, 1.0);

    constexpr f32 GZ_FLOAT_EPSILON = std::numeric_limits<f32>::epsilon();

    constexpr vec3 GZ_VEC3_FLOAT_EPSILON = {GZ_FLOAT_EPSILON, GZ_FLOAT_EPSILON, GZ_FLOAT_EPSILON};

	GZ_FORCE_INLINE f64 get_s_from_ns(u64 ns) {
		return static_cast<f64>(ns) / SDL_NS_PER_SECOND;
	}

	GZ_FORCE_INLINE f64 get_ms_from_ns(u64 ns) {
		return static_cast<f64>(ns) / SDL_NS_PER_MS;
	}

	GZ_FORCE_INLINE f64 get_s_from_ms(u64 ms) {
		return static_cast<f64>(ms) / SDL_MS_PER_SECOND;
	}

	GZ_FORCE_INLINE u64 get_ns_from_s(f64 s) {
		return static_cast<u64>(s * SDL_NS_PER_SECOND);
	}

	GZ_FORCE_INLINE u64 get_ns_from_ms(f64 ms) {
		return static_cast<u64>(ms * SDL_NS_PER_MS);
	}

    constexpr f32 f_epsilon = std::numeric_limits<f32>::epsilon();
    constexpr f32 scale_factor = 1;
    constexpr f32 tolerance_factor = 1;
    GZ_FORCE_INLINE b8 is_approximately_equal(float a, float b) {
        double scale = std::fmax(abs(a), abs(b));
        return abs(a - b) < scale_factor * (tolerance_factor * f_epsilon);
    }

    GZ_FORCE_INLINE b8 is_approximately_zero(float val) {
        return abs(val) < scale_factor * (tolerance_factor * f_epsilon);
    }

    GZ_FORCE_INLINE b8 is_vec3_all_equal_epsilon(const vec3& a, const vec3& b) {
        auto rets = glm::epsilonEqual(a, b, GZ_VEC3_FLOAT_EPSILON);
        return rets[0] && rets[1] && rets[2];
    }

    // no normalization
    // we are doing everything in degrees, so convert to radians
    GZ_FORCE_INLINE quat to_quat(const vec3 &euler_angles) {
        return quat(glm::radians((euler_angles)));
    }

    GZ_FORCE_INLINE vec3 to_euler(const quat& normalized_qua) {
        return glm::degrees(glm::eulerAngles(normalized_qua));
    }

	GZ_FORCE_INLINE float normalize_angle(float degrees) {
		// 1. Wrap to [0, 360) range
		degrees = std::fmod(degrees, 360.0f);

		// 2. Shift to [-180, 180) for better UI display
		if (degrees > 180.0f) {
			degrees -= 360.0f;
		}
		else if (degrees < -180.0f) {
			degrees += 360.0f;
		}

		// Optional: Snap near-threshold values to prevent jitter
		const float epsilon = 0.001f;
		if (std::abs(degrees - 180.0f) < epsilon) {
			degrees = -180.0f;
		}
		if (std::abs(degrees + 180.0f) < epsilon) {
			degrees = 180.0f;
		}

		return degrees;
	}

	GZ_FORCE_INLINE glm::vec3 normalize_angles(glm::vec3 euler_deg) {
		return glm::vec3(
			normalize_angle(euler_deg.x),
			normalize_angle(euler_deg.y),
			normalize_angle(euler_deg.z)
		);
	}
}

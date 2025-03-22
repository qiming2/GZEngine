#pragma once
#include <SDL3/SDL.h>

namespace GZ {

	GZ_FORCE_INLINE f64 get_s_from_ns(u64 ns) {
		return static_cast<f64>(ns) / SDL_NS_PER_SECOND;
	}

	GZ_FORCE_INLINE f64 get_ms_from_ns(u64 ns) {
		return static_cast<f64>(ns) / SDL_NS_PER_MS;
	}

	GZ_FORCE_INLINE f64 get_s_from_ms(u64 ms) {
		return static_cast<f64>(ms) / SDL_MS_PER_SECOND;
	}

}
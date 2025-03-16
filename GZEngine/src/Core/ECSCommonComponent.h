#include <gzpch.h>

#include "Log.h"
namespace GZ {
	struct Transform {
		vec3 position;
		vec3 scale;
		vec3 rotation;
	};
}

template<>
struct fmt::formatter<GZ::Transform> : fmt::formatter<std::string>
{
	auto format(GZ::Transform my_transform, format_context& ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "\np: {}\ns: {}\nr: {}", my_transform.position, my_transform.scale, my_transform.rotation);
	}
};

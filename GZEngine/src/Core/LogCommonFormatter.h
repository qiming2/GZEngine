#include <gzpch.h>
#include "spdlog/spdlog.h"

template<>
struct fmt::formatter<GZ::vec4> : fmt::formatter<std::string>
{
	auto format(GZ::vec4 my_type, format_context& ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "[{:.4f} {:.4f} {:.4f} {:.4f}]", my_type.x, my_type.y, my_type.z, my_type.w);
	}
};

template<>
struct fmt::formatter<GZ::quat> : fmt::formatter<std::string>
{
    auto format(GZ::quat my_type, format_context& ctx) const -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "[{:.4f} {:.4f} {:.4f} {:.4f}]", my_type.x, my_type.y, my_type.z, my_type.w);
    }
};

template<>
struct fmt::formatter<GZ::vec3> : fmt::formatter<std::string>
{
	auto format(GZ::vec3 my_type, format_context& ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "[{:.4f} {:.4f} {:.4f}]", my_type.x, my_type.y, my_type.z);
	}
};

template<>
struct fmt::formatter<GZ::vec2> : fmt::formatter<std::string>
{
	auto format(GZ::vec2 my_type, format_context& ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "[{:.4f} {:.4f}]", my_type.x, my_type.y);
	}
};

template<>
struct fmt::formatter<GZ::mat4> : fmt::formatter<std::string>
{
	auto format(GZ::mat4 my_type, format_context& ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "\n{:.4f} {:.4f} {:.4f} {:.4f}\n{:.4f} {:.4f} {:.4f} {:.4f}\n{:.4f} {:.4f} {:.4f} {:.4f}\n{:.4f} {:.4f} {:.4f} {:.4f}", 
			my_type[0][0], my_type[1][0], my_type[2][0], my_type[3][0],
			my_type[0][1], my_type[1][1], my_type[2][1], my_type[3][1],
			my_type[0][2], my_type[1][2], my_type[2][2], my_type[3][2],
			my_type[0][3], my_type[1][3], my_type[2][3], my_type[3][3]);
	}
};

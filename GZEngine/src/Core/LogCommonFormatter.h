#include <gzpch.h>
#include "spdlog/spdlog.h"

template<>
struct fmt::formatter<GZ::vec4> : fmt::formatter<std::string>
{
	auto format(GZ::vec4 my_vec, format_context& ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "[{:.4f} {:.4f} {:.4f} {:.4f}]", my_vec.x, my_vec.y, my_vec.z, my_vec.w);
	}
};

template<>
struct fmt::formatter<GZ::vec3> : fmt::formatter<std::string>
{
	auto format(GZ::vec3 my_vec, format_context& ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "[{:.4f} {:.4f} {:.4f}]", my_vec.x, my_vec.y, my_vec.z);
	}
};

template<>
struct fmt::formatter<GZ::vec2> : fmt::formatter<std::string>
{
	auto format(GZ::vec2 my_vec, format_context& ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "[{:.4f} {:.4f}]", my_vec.x, my_vec.y);
	}
};

template<>
struct fmt::formatter<GZ::mat4> : fmt::formatter<std::string>
{
	auto format(GZ::mat4 my_mat, format_context& ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "\n{:.4f} {:.4f} {:.4f} {:.4f}\n{:.4f} {:.4f} {:.4f} {:.4f}\n{:.4f} {:.4f} {:.4f} {:.4f}\n{:.4f} {:.4f} {:.4f} {:.4f}", 
			my_mat[0][0], my_mat[1][0], my_mat[2][0], my_mat[3][0],
			my_mat[0][1], my_mat[1][1], my_mat[2][1], my_mat[3][1],
			my_mat[0][2], my_mat[1][2], my_mat[2][2], my_mat[3][2],
			my_mat[0][3], my_mat[1][3], my_mat[2][3], my_mat[3][3]);
	}
};
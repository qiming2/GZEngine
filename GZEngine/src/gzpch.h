#ifndef GZPCH_H
#define GZPCH_H

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdlib>
#include <cstdarg>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <random>
#include <set>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <typeinfo>
#include <utility>
#include <vector>
#include <filesystem>
#include <thread>
#include <optional>
#include <span>

#include <defines.h>

#if defined(__clang__)
#define GZ_FORCE_INLINE [[clang::always_inline]] inline

#elif defined(__GNUC__)
#define GZ_FORCE_INLINE [[gnu::always_inline]] inline

#elif defined(_MSC_VER)
#pragma warning(error: 4714)
#define GZ_FORCE_INLINE __forceinline
#else
#error Unsupported compiler
#endif
////////////////////////////////////// Vendor ///////////////////////////////////////
// All glm defines are predefined in cmake
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <flecs.h>
// glm alias
namespace GZ {

	// Some convenience using
	using vec4 = glm::vec4;
	using vec3 = glm::vec3;
	using vec2 = glm::vec2;
	
	using ivec4 = glm::ivec4;
	using ivec3 = glm::ivec3;
	using ivec2 = glm::ivec2;

	using uvec4 = glm::uvec4;
	using uvec3 = glm::uvec3;
	using uvec2 = glm::uvec2;

	using mat4 = glm::mat4;
	using mat3 = glm::mat3;
	using mat2 = glm::mat2;

	using quat = glm::quat;

	using World = flecs::world;
	using ScopedWorld = flecs::scoped_world;
	using Entity = flecs::entity;
	using Prefab = flecs::entity;
	using System = flecs::system;
	using Observer = flecs::observer;
	using WorldIter = flecs::iter;
	using Identifier = flecs::id;
	using IdentifierID = flecs::id_t;
	using EntityID = flecs::entity_t;
	using WorldID = flecs::world_t;
	using PrefabID = flecs::entity_t;
	using ComponentID = flecs::entity_t;
}

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_node_editor.h>

#include <Jolt/Jolt.h>

// Don't put engine header!!!!! Circular dependency

#endif

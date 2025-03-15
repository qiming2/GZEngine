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
#include <utility>
#include <vector>
#include <filesystem>
#include <thread>
#include <optional>

#include <defines.h>

// All glm defines are predefined in cmake
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// stb
#include <stb_image.h>

// glm alias
namespace GZ {
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

	using quatf = glm::quat;
}


#endif
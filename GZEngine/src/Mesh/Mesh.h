#pragma once
#include <gzpch.h>

// Temporary
#include <vulkan/vulkan.h>
namespace GZ {
	
	// Mesh Related
	// Push Constant has 128 bytes alignment
	struct PerObjectPushConstant {
		mat4 model;
	};
	// uniform object
	struct UniformBufferObject {
		mat4 view;
		mat4 proj;
	};

	struct Vertex {
		vec3 pos;
		vec3 color;
		vec2 uv;

		static VkVertexInputBindingDescription getBindingDescription() {
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(Vertex, uv);
			return attributeDescriptions;
		}
	};

	// Always generate CCW indices buffer, and always tessalated
	struct Mesh {
    public:
		Mesh();
		Mesh(std::vector<Vertex> &vertices, std::vector<u32> &indices);
		~Mesh();
		
		GZ_FORCE_INLINE const std::vector<Vertex>& get_vertex_buffer() const {
			return m_vertex_buffer;
		}

		GZ_FORCE_INLINE const std::vector<u32>& get_index_buffer() const {
			return m_index_buffer;
		}
        
        GZ_FORCE_INLINE void set_vertex_buffer(VkBuffer vbuffer) {
            this->vbuffer = vbuffer;
        }
        
        GZ_FORCE_INLINE void set_index_buffer(VkBuffer ibuffer) {
            this->ibuffer = ibuffer;
        }
        
		static std::shared_ptr<Mesh> get_icosphere_mesh(f32 radius = 0.5f, i32 recursion_level = 5);
		static std::shared_ptr<Mesh> get_box_mesh(vec3 extent = {0.5f, 0.5f, 0.5f});
        
        static std::shared_ptr<Mesh> get_uvsphere_mesh(f32 radius = 0.5f, i32 num_stack = 32, i32 num_slice = 64);
        
        static std::shared_ptr<Mesh> load_mesh_from_obj(const std::string_view &path);
	private:
		std::vector<Vertex> m_vertex_buffer;
		std::vector<u32> m_index_buffer;
        
    public: // Runtime only and cleaned up by the renderer for now
        VkBuffer vbuffer;
        VkBuffer ibuffer;
	};

}

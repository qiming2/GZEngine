#pragma once
#include <gzpch.h>
#include <imgui_impl_vulkan.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>

namespace GZ {

	// uniform object
	struct UniformBufferObject {
		glm::mat4 model; // this should be push constants
		glm::mat4 view;
		glm::mat4 proj;
	};

	// Mesh Related
	struct Vertex {
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 uv;

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
	
	struct QueueFamilyIndices {
		std::optional<u32> graphicsFamily;
		std::optional<u32> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};
	struct Renderer {
	public:
		b8 init(void *window_handle);
		b8 deinit();
		void init_imgui_vulkan(ImGui_ImplVulkan_InitInfo *init_info);

		// Generic draw function right now
		void begin_frame(const f32 &deltaTime);
		void render_frame();
		void end_frame();
		void set_imgui_draw_data(ImDrawData *imgui_data);
		void will_deinit();
        void handle_window_resized();
	private:
		VkInstance instance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
		VkSurfaceKHR surface = VK_NULL_HANDLE;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device = VK_NULL_HANDLE;
		VkQueue presentQueue;
		VkQueue graphicsQueue = VK_NULL_HANDLE;

		// Swapchain specific member variables
		VkSwapchainKHR swapChain = VK_NULL_HANDLE;
		std::vector<VkImage> swapChainImages;
		std::vector<VkImageView> swapChainImageViews;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;

		VkPipeline graphicsPipeline;
		VkRenderPass renderPass;

		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;
		VkDescriptorSetLayout descriptorSetLayout;
		VkPipelineLayout pipelineLayout;

		std::vector<VkFramebuffer> swapChainFramebuffers;

		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;

		// Sync objects
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;

		void *window_handle; // Platform specific
		
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;
		
		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;
		std::vector<void*> uniformBuffersMapped;

		// Image textures
		VkImage textureImage;
		VkDeviceMemory textureImageMemory;
		VkImageView textureImageView;
		VkSampler textureSampler;

		u32 current_frame_index = 0;
		static const int MAX_FRAMES_IN_FLIGHT = 2;
	private: // Provided by application
		// Imgui draw list
		ImDrawData *imgui_data = nullptr;
		f32	accumulatedTime = 0.0f;
		f32 deltaTime = 0.0f;
	private: // Can expect the order to be the same order as defined here

		// order do matter for most of later functions which
		// depend on previous vk objects
		void create_instance();
		void setup_debug_messenger();
		void create_surface();
		void pick_physical_device();
		void create_logical_device();
		void create_swapchain();
		void create_swapchain_image_views();
		void create_render_pass();
		void create_descriptor_set_layout();
		void create_graphics_pipeline();
		void create_framebuffers();
		void create_command_pool();
		void create_texture_image();
		void create_texture_image_view();
		void create_texture_sampler();
		void create_vertex_buffer();
		void create_index_buffer();
		void create_uniform_buffer();
		void create_descriptor_pool();
		void create_descriptor_sets();
		void create_command_buffer();
		void create_sync_objects();

		// Render command potentially
		void record_command_buffer(VkCommandBuffer commandBuffer, u32 imageIndex);
        
        void recreate_swapchain();
        void cleanup_swapchain();
		void update_uniform_buffer(u32 current_frame_index);
	private: // helper util
		SwapChainSupportDetails query_swapchain_support(VkPhysicalDevice device);
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		VkShaderModule createShaderModule(const std::vector<char>& code);

		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		u32 rateDevice(VkPhysicalDevice device);
		u32 findMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties);
		
		// Transition image layout
		void transition_image_layout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

		void copy_buffer_to_image(VkBuffer buffer, VkImage image, u32 width, u32 height);

		// single time command
		VkCommandBuffer begin_single_time_commands();
		void end_single_time_commands(VkCommandBuffer commandBuffer);

		void create_image(u32 width, u32 height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

		VkImageView create_image_view(VkImage image, VkFormat format);

		// buffer related
		void create_buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void copy_buffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	private: // helper member var
		QueueFamilyIndices queue_family_indices;

	};


}

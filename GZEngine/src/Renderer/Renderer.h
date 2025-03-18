#pragma once
#include <gzpch.h>
#include <imgui_impl_vulkan.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>

#include "Mesh.h"

namespace GZ {

	// uniform object
	struct UniformBufferObject {
		glm::mat4 model; // this should be push constants
		glm::mat4 view;
		glm::mat4 proj;
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
	public: // Hack things together
		void set_model_matrix(const mat4& model);
	private:
		mat4 m_model = mat4(1.0f);
	public:
		b8 init(void *window_handle);
		b8 deinit();
		void init_imgui_vulkan(ImGui_ImplVulkan_InitInfo *init_info);
		void* get_main_color_texture_imgui_id();
        u32 get_min_image_count();

		// Generic draw function right now
		void begin_frame(const f32 &deltaTime);
		
		void end_frame();
		void set_imgui_draw_data(ImDrawData *imgui_data);
		void will_deinit();
        void handle_window_resized();
		void set_viewport_size(u32 w, u32 h);
		void set_clear_value(vec4 clear_color = vec4(0.18, 0.18, 0.18, 1.0));
	public: // Geometry related
		void submit_mesh(std::shared_ptr<Mesh> mesh);
	private:
		// temp mesh for drawing
		std::vector<std::shared_ptr<Mesh>> m_meshes;
		std::vector<VkBuffer> m_mesh_vertex_buffers;
		std::vector<VkDeviceMemory> m_mesh_vertex_buffer_memories;
		std::vector<VkBuffer> m_mesh_index_buffers;
		std::vector<VkDeviceMemory> m_mesh_index_buffer_memories;
		
	private: // Vk context
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
		
		VkBuffer m_vertexBuffer;
		VkDeviceMemory m_vertexBufferMemory;
		VkBuffer m_indexBuffer;
		VkDeviceMemory m_indexBufferMemory;
		
		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;
		std::vector<void*> uniformBuffersMapped;

		VkImage depthImage;
		VkDeviceMemory depthImageMemory;
		VkImageView depthImageView;

		// Image textures
		u32 mipLevels;
		VkImage textureImage;
		VkDeviceMemory textureImageMemory;
		VkImageView textureImageView;
		VkSampler textureSampler;

		// msaa color attachment
		VkImage colorImage;
		VkDeviceMemory colorImageMemory;
		VkImageView colorImageView;

		// final blit output image for sampling
		VkImage outColorImage;
		VkDeviceMemory outColorImageMemory;
		VkImageView outColorImageView;

		VkPipeline offScreenPipeline;
		VkRenderPass offScreenRenderPass;

		VkFramebuffer offScreenFrameBuffer;

		VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

	private: // Configurable value
		// viewport w, h
		u32 viewport_w = 0, viewport_h = 0;

		vec4 clear_color = vec4(0.18, 0.18, 0.18, 1.0);

		u32 current_frame_index = 0;
		static const int MAX_FRAMES_IN_FLIGHT = 2;
    private: // temp test model
        // Test obj:
        const uint32_t viking_texture_width = 800;
        const uint32_t viking_texture_height = 600;

        std::vector<Vertex> m_vertices;
        std::vector<uint32_t> m_indices;

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
		void create_offscreen_render_pass();
		void create_descriptor_set_layout();
		void create_graphics_pipeline();
		void create_offscreen_graphics_pipeline();
		void create_final_framebuffers();
		void create_offscreen_framebuffer();
		void create_command_pool();
		void create_color_resources();
		void create_out_color_resources();
		void create_depth_resources();
		void create_texture_image();
		void create_texture_image_view();
		void create_texture_sampler();
        void load_model();
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
        void render_frame();
	private: // helper util
		SwapChainSupportDetails query_swapchain_support(VkPhysicalDevice device);
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		VkShaderModule createShaderModule(const std::vector<char>& code);

		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		u32 rateDevice(VkPhysicalDevice device);
		u32 findMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties);

		VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

		VkFormat find_depth_format();

		// Transition image layout
		void transition_image_layout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, u32 mipLevels);

		void copy_buffer_to_image(VkBuffer buffer, VkImage image, u32 width, u32 height);

		// single time command
		VkCommandBuffer begin_single_time_commands();
		void end_single_time_commands(VkCommandBuffer commandBuffer);

		void create_image(u32 width, u32 height, u32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

		VkImageView create_image_view(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, u32 mipLevels);

		// buffer related
		void create_buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void copy_buffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void generate_mipmaps(VkImage image, VkFormat imageFormat, i32 texWidth, i32 texHeight, u32 mipLevels);

		VkSampleCountFlagBits get_max_usable_sample_count();
	private: // helper member var
		QueueFamilyIndices queue_family_indices;

	};


}

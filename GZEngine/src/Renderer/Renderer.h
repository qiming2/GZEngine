#pragma once
#include <gzpch.h>
#include <imgui_impl_vulkan.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>

namespace GZ {
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
		void render_frame();
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
		VkPipelineLayout pipelineLayout;

		std::vector<VkFramebuffer> swapChainFramebuffers;

		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;

		// Sync objects
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;

		void *window_handle; // Platform specific
		
		// Imgui draw list
		ImDrawData *imgui_data = nullptr;
        
        u32 current_frame_index = 0;
        static const int MAX_FRAMES_IN_FLIGHT = 2;
	private: // Can expect the order to be the same order as defined here
		void create_instance();
		void setup_debug_messenger();
		void create_surface();
		void pick_physical_device();
		void create_logical_device();
		void create_swapchain();
		void create_swapchain_image_views();
		void create_render_pass();
		void create_graphics_pipeline();
		void create_framebuffers();
		void create_command_pool();
		void create_command_buffer();
		void create_sync_objects();

		// Render command potentially
		void record_command_buffer(VkCommandBuffer commandBuffer, u32 imageIndex);
        
        void recreate_swapchain();
        void cleanup_swapchain();
        
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	private: // helper util
		SwapChainSupportDetails query_swapchain_support(VkPhysicalDevice device);
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		VkShaderModule createShaderModule(const std::vector<char>& code);

		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		b8 isDeviceSuitable(VkPhysicalDevice device);
	private: // helper member var
		QueueFamilyIndices queue_family_indices;

	};


}

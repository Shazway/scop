/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   device.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 11:35:42 by tmoragli          #+#    #+#             */
/*   Updated: 2024/02/21 22:20:22 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "window.hpp"
#include "colors.hpp"


namespace scop {

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct QueueFamilyIndices {
		uint32_t graphicsFamily;
		uint32_t presentFamily;
		bool graphicsFamilyHasValue = false;
		bool presentFamilyHasValue = false;
		bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
	};

	class Device {
		public:
			#ifdef NDEBUG
				const bool enableValidationLayers = false;
			#else
				const bool enableValidationLayers = true;
			#endif

			Device(Window &window);
			~Device();

			// Not copyable or movable
			Device(const Device &) = delete;
			Device& operator=(const Device &) = delete;
			Device(Device &&) = delete;
			Device &operator=(Device &&) = delete;

			VkCommandPool getCommandPool() { return commandPool; }
			VkDevice device() { return device_; }
			VkSurfaceKHR surface() { return surface_; }
			VkQueue graphicsQueue() { return graphicsQueue_; }
			VkQueue presentQueue() { return presentQueue_; }

			SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice); }
			uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
			QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice); }
			VkFormat findSupportedFormat(
					const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

			// Buffer Helper Functions
			void createBuffer(
					VkDeviceSize size,
					VkBufferUsageFlags usage,
					VkMemoryPropertyFlags properties,
					VkBuffer &buffer,
					VkDeviceMemory &bufferMemory);
			VkCommandBuffer beginSingleTimeCommands();
			void endSingleTimeCommands(VkCommandBuffer commandBuffer);
			void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
			void copyBufferToImage(
					VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

			void createImageWithInfo(
					const VkImageCreateInfo &imageInfo,
					VkMemoryPropertyFlags properties,
					VkImage &image,
					VkDeviceMemory &imageMemory);

			VkPhysicalDeviceProperties properties;

		private:
			void createInstance();
			void setupDebugMessenger();
			void createSurface();
			void pickPhysicalDevice();
			void createLogicalDevice();
			void createCommandPool();

			// helper functions
			bool isDeviceSuitable(VkPhysicalDevice Device);
			std::vector<const char *> getRequiredExtensions();
			bool checkValidationLayerSupport();
			QueueFamilyIndices findQueueFamilies(VkPhysicalDevice Device);
			void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
			void hasGflwRequiredInstanceExtensions();
			bool checkDeviceExtensionSupport(VkPhysicalDevice Device);
			SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice Device);

			VkInstance instance;
			VkDebugUtilsMessengerEXT debugMessenger;
			VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
			Window &window;
			VkCommandPool commandPool;

			VkDevice device_;
			VkSurfaceKHR surface_;
			VkQueue graphicsQueue_;
			VkQueue presentQueue_;

			const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
			const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
	};
}  // namespace scop
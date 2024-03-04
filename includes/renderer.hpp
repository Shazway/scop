/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 14:38:46 by tmoragli          #+#    #+#             */
/*   Updated: 2024/03/04 15:27:53 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

#include "window.hpp"
#include "device.hpp"
#include "swap_chain.hpp"
#include "model.hpp"

#include <memory>
#include <vector>

namespace scop {

	class Renderer {
		private:
			Window&	_window;
			Device&	_device;
			std::unique_ptr<SwapChain>	swap_chain;
			std::vector<VkCommandBuffer> commandBuffers;
			uint32_t currentImageIndex;
			int currentFrameIndex;
			bool isFrameStarted;
		public:
			Renderer(Window& window, Device& device);
			~Renderer();

			VkCommandBuffer beginFrame();
			void endFrame();
			void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
			void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
			VkRenderPass getSwapChainRenderPass() const {return swap_chain->getRenderPass();};
			bool isFrameInProgress() const { return isFrameStarted; };
			VkCommandBuffer getCurrentCommandBuffer() const {
				assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
				return commandBuffers[currentFrameIndex];
			};
			int getFrameIndex() const {
				assert(isFrameStarted && "Cannot get frame index when frame not in progress");
				return currentFrameIndex;
			}

			Renderer(Renderer const& window) = delete;
			Renderer &operator=(Renderer const) = delete;
		private:
			void createCommandBuffers();
			void freeCommandBuffers();
			void recreateSwapChain();
	};

} // namespace scop
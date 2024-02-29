/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 23:03:56 by tmoragli          #+#    #+#             */
/*   Updated: 2024/02/23 22:00:30 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_HPP
#define APP_HPP

#include "window.hpp"
#include "pipeline.hpp"
#include "device.hpp"
#include "swap_chain.hpp"
#include "model.hpp"
#include <memory>
#include <vector>

#define WIDTH 1080
#define HEIGHT 760

namespace scop {

	class App {
		private:
			Window	_window{WIDTH, HEIGHT, "Scop"};
			Device	_device{_window};
			std::unique_ptr<SwapChain>	swap_chain;
			std::unique_ptr<Pipeline>	_pipeline;
			std::unique_ptr<Model>	_model;
			VkPipelineLayout pipelineLayout;
			std::vector<VkCommandBuffer> commandBuffers;
		public:
			App();
			~App();
			App(App const& window) = delete;
			App &operator=(App const) = delete;
			void run();
		private:
			void loadModels();
			void createPipelineLayout();
			void createPipeline();
			void createCommandBuffers();
			void freeCommandBuffers();
			void drawFrame();
			void recreateSwapChain();
			void recordCommandBuffer(int imageIndex);
	};

} // namespace scop

#endif
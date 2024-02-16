/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 23:03:56 by tmoragli          #+#    #+#             */
/*   Updated: 2024/02/11 17:34:41 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_HPP
#define APP_HPP

#include "window.hpp"
#include "pipeline.hpp"
#include "device.hpp"
#include "swap_chain.hpp"
#include <memory>
#include <vector>

#define WIDTH 1080
#define HEIGHT 760

namespace scop {

	class App {
		private:
			Window	_window{WIDTH, HEIGHT, "Scop"};
			Device	_device{_window};
			SwapChain	swap_chain{_device, _window.getExtent()};
			std::unique_ptr<Pipeline>	_pipeline;
			VkPipelineLayout pipelineLayout;
			std::vector<VkCommandBuffer> commandBuffers;
		public:
			App();
			~App();
			App(App const& window) = delete;
			App &operator=(App const) = delete;
			void run();
		private:
			void createPipelineLayout();
			void createPipeline();
			void createCommandBuffers();
			void drawFrame();
	};

} // namespace scop

#endif
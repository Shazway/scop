/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_render_system.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 15:09:12 by tmoragli          #+#    #+#             */
/*   Updated: 2024/03/09 21:17:41 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "pipeline.hpp"
#include "device.hpp"
#include "model.hpp"
#include "scop_object.hpp"
#include "camera.hpp"
#include <memory>
#include <vector>

namespace scop {

	class SimpleRenderSystem {
		private:
			Device&	_device;
			std::unique_ptr<Pipeline>	_pipeline;
			VkPipelineLayout pipelineLayout;
		public:
			SimpleRenderSystem(Device& device, VkRenderPass renderPass);
			~SimpleRenderSystem();
			SimpleRenderSystem(SimpleRenderSystem const& window) = delete;
			SimpleRenderSystem &operator=(SimpleRenderSystem const) = delete;
			void renderScopObjects(VkCommandBuffer commandBuffer, std::vector<ScopObject>& scopObjects, Camera const& camera);
		private:
			void createPipelineLayout();
			void createPipeline(VkRenderPass renderPass);
	};

} // namespace scop
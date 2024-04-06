/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_render_system.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 15:12:17 by tmoragli          #+#    #+#             */
/*   Updated: 2024/03/09 22:55:22 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


namespace scop {

	struct SimplePushConstantData {
		glm::mat4 transform{1.f};
		alignas(16) glm::vec3 color;
	};

	SimpleRenderSystem::SimpleRenderSystem(Device& device, VkRenderPass renderPass) : _device{device} {
		createPipelineLayout();
		createPipeline(renderPass);
	}
	SimpleRenderSystem::~SimpleRenderSystem() {
		vkDestroyPipelineLayout(_device.device(), pipelineLayout, nullptr);
	}

	void SimpleRenderSystem::createPipelineLayout()
	{
		VkPushConstantRange pushConstantRange {};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		if (vkCreatePipelineLayout(_device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
			throw std::runtime_error("failed to create pipeline layout !");
	}

	void SimpleRenderSystem::createPipeline(VkRenderPass renderPass) {
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		Pipeline::defaultPipelineConfigInfo(pipelineConfig);

		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		_pipeline = std::make_unique<Pipeline>(
			_device,
			"vert.spv",
			"frag.spv",
			pipelineConfig);
	}


	void SimpleRenderSystem::renderScopObjects(VkCommandBuffer commandBuffer, std::vector<ScopObject> &scopObjects, Camera const& camera) {
		_pipeline->bind(commandBuffer);

		auto projectionView = camera.getProjection() * camera.getView();

		for (auto& object: scopObjects) {
			// Add spin to the objects
			// object.transform.rotation.y = glm::mod(object.transform.rotation.y + 0.01f, glm::two_pi<float>());
			// object.transform.rotation.x = glm::mod(object.transform.rotation.x + 0.005f, glm::two_pi<float>());
			
			SimplePushConstantData push {};

			push.color = object.color;
			push.transform = projectionView * object.transform.mat4();

			vkCmdPushConstants(commandBuffer,
								pipelineLayout,
								VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
								0,
								sizeof(SimplePushConstantData),
								&push
							);
			object.model->bind(commandBuffer);
			object.model->draw(commandBuffer);
		}
	}
}

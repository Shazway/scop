/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 22:42:42 by tmoragli          #+#    #+#             */
/*   Updated: 2024/02/23 22:00:30 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace scop {

	struct SimplePushConstantData {
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};

	void App::run() {
		while (!_window.shouldClose()) {
			glfwPollEvents();
			drawFrame();
		}

		vkDeviceWaitIdle(_device.device());
	}

	App::App() {
		loadModels();
		createPipelineLayout();
		recreateSwapChain();
		createCommandBuffers();
	}
	App::~App() {
		vkDestroyPipelineLayout(_device.device(), pipelineLayout, nullptr);
	}

	void sierpinski(int depth, std::vector<Model::Vertex> &vertices, glm::vec2 left, glm::vec2 right, glm::vec2 top)
	{
		if (depth <= 0)
		{
			vertices.push_back({right, {1.0f, 0.0f, 0.0f}});
			vertices.push_back({left, {0.0f, 1.0f, 0.0f}});
			vertices.push_back({top, {0.0f, 0.0f, 1.0f}});
			return ;
		}
		auto leftTop = 0.5f * (left + top);
		auto rightTop = 0.5f * (right + top);
		auto leftRight = 0.5f * (left + right);
		sierpinski(depth - 1, vertices, left, leftRight, leftTop);
		sierpinski(depth - 1, vertices, leftRight, right, rightTop);
		sierpinski(depth - 1, vertices, leftTop, rightTop, top);
	}

	void App::loadModels()
	{
		std::vector<Model::Vertex> vertices
		{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};

		//sierpinski(4, vertices, {-1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, -1.0f});
		_model = std::make_unique<Model>(_device, vertices);
	}

	void App::createPipelineLayout()
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

	void App::createPipeline() {
		assert(swap_chain != nullptr && "Cannot create pipeline before swap chain");
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");
		PipelineConfigInfo pipelineConfig{};
		Pipeline::defaultPipelineConfigInfo(pipelineConfig);

		pipelineConfig.renderPass = swap_chain->getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		_pipeline = std::make_unique<Pipeline>(
			_device,
			"shaders/vert.spv",
			"shaders/frag.spv",
			pipelineConfig);
	}

	void App::recreateSwapChain() {
		auto extent = _window.getExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = _window.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(_device.device());
		//swap_chain.reset(nullptr);
		if (swap_chain == nullptr) {
			swap_chain = std::make_unique<SwapChain>(_device, extent);
		}
		else
		{
			swap_chain = std::make_unique<SwapChain>(_device, extent, std::move(swap_chain));
			if (swap_chain->imageCount() != commandBuffers.size()) {
				freeCommandBuffers();
				createCommandBuffers();
			}
		}
		createPipeline();
	}

	void App::createCommandBuffers()
	{
		commandBuffers.resize(swap_chain->imageCount());

		VkCommandBufferAllocateInfo allocInfo{};

		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = _device.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(_device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
			throw std::runtime_error("failed to allocate command buffers!");
	}

	void App::freeCommandBuffers() {
		vkFreeCommandBuffers(_device.device(), _device.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
		commandBuffers.clear();
	}

	void App::recordCommandBuffer(int imageIndex) {
		static int frame = 0;
		frame = (frame + 1) % 100;

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
			throw std::runtime_error("failed to begin recording command buffer!");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = swap_chain->getRenderPass();
		renderPassInfo.framebuffer = swap_chain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = swap_chain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
		clearValues[1].depthStencil = {1.0f, 0};

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(swap_chain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(swap_chain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor {{0, 0}, swap_chain->getSwapChainExtent()};
		vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

		_pipeline->bind(commandBuffers[imageIndex]);
		_model->bind(commandBuffers[imageIndex]);

		for (int j = 0; j < 4; j++) {
			SimplePushConstantData push {};
			push.offset = {-0.5f + frame * 0.02f, -0.4f + j * 0.25f};
			push.color = {0.0f, 0.0f, 0.2f + 0.2f * j};

			vkCmdPushConstants(commandBuffers[imageIndex],
								pipelineLayout,
								VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
								0,
								sizeof(SimplePushConstantData),
								&push
							);
			_model->draw(commandBuffers[imageIndex]);
		}

		vkCmdEndRenderPass(commandBuffers[imageIndex]);
		if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
			throw std::runtime_error("failed to record command buffer!");
	}

	void App::drawFrame()
	{
		uint32_t imageIndex;

		auto result = swap_chain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return ;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			throw std::runtime_error("failed to acquire swap chain index");

		recordCommandBuffer(imageIndex);
		result = swap_chain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _window.wasWindowResized())
		{
			_window.resetWindowResizedFlag();
			recreateSwapChain();
			return ;
		}
		if (result != VK_SUCCESS)
			throw std::runtime_error("failed to present swap chain image!");
	}

} // scop namespace
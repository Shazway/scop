/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 10:13:05 by tmoragli          #+#    #+#             */
/*   Updated: 2024/02/15 20:28:04 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "colors.hpp"
#include "device.hpp"

namespace scop {
	
	struct PipelineConfigInfo {
		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class Pipeline {
		public:
			Pipeline(Device &device, std::string const& vertFilePath, std::string const& fragFilePath, PipelineConfigInfo const& configInfo);
			Pipeline(const Pipeline& pipeline) = delete;
			void operator=(const Pipeline& pipeline) = delete;
			~Pipeline();

			static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);
			void   bind(VkCommandBuffer commandBuffer);
		private:
			static std::vector<char> readFile(std::string const& filePath);
			void createGraphicsPipeline(std::string const& vertFilePath, std::string const& fragFilePath, PipelineConfigInfo const& configInfo);
			void createShaderModule(std::vector<char> const& code, VkShaderModule *shaderModule);
		private:
			Device	&device;
			VkPipeline		graphicsPipeline;
			VkShaderModule	vertShaderModule;
			VkShaderModule	fragShaderModule;
	};
}// scop namespace

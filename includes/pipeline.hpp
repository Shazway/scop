/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 10:13:05 by tmoragli          #+#    #+#             */
/*   Updated: 2024/02/22 21:24:16 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "colors.hpp"
#include "device.hpp"

namespace scop {
	
	struct PipelineConfigInfo {
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(PipelineConfigInfo const&) = delete;

		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class Pipeline {
		public:
			Pipeline() = default;
			Pipeline(Device &device, std::string const& vertFilePath, std::string const& fragFilePath, PipelineConfigInfo const& configInfo);
			Pipeline(const Pipeline& pipeline) = delete;
			Pipeline& operator=(const Pipeline& pipeline) = delete;
			~Pipeline();

			static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
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

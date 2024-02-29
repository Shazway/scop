/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   model.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 00:33:56 by tmoragli          #+#    #+#             */
/*   Updated: 2024/02/20 23:01:59 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "device.hpp"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vector>
#include "colors.hpp"
#include <cstring>

namespace scop {
	class Model {
		public:
			struct Vertex {
				glm::vec2 position;
				glm::vec3 color;

				static std::vector<VkVertexInputBindingDescription> getBindingDescription();
				static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
			};
		private:
			Device &device;
			VkBuffer vertexBuffer;
			VkDeviceMemory vertexBufferMemory;
			uint32_t vertexCount;
		public:
			Model(Device &newDevice, std::vector<Vertex> const& vertices);
			~Model();
			Model(Model const& window) = delete;
			Model &operator=(Model const) = delete;
	
			void bind(VkCommandBuffer commandBuffer);
			void draw(VkCommandBuffer commandBuffer);
		private:
			void createVertexBuffers(std::vector<Vertex> const& vertices);
	};
}
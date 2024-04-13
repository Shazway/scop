/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   model.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 00:33:56 by tmoragli          #+#    #+#             */
/*   Updated: 2024/04/11 23:48:59 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "device.hpp"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vector>
#include "colors.hpp"
#include "geometry_parser.hpp"
#include <cstring>
#include <memory>


namespace scop {
	class Model {
		public:
			struct Vertex {
				glm::vec3 position{};
				glm::vec3 color{};
				glm::vec3 normal{};
				glm::vec2 uv{};

				bool operator==(Vertex const& other) const {
					return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
				}
				static std::vector<VkVertexInputBindingDescription> getBindingDescription();
				static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
			};

			struct Builder {
				std::vector<Vertex> vertices {};
				std::vector<uint32_t> indices {}; 
				void loadModel(std::string const& filePath);
			};

		private:
			Device &device;
			VkBuffer vertexBuffer;
			VkDeviceMemory vertexBufferMemory;
			uint32_t vertexCount;

			bool hasIndexBuffer = false;
			VkBuffer indexBuffer;
			VkDeviceMemory indexBufferMemory;
			uint32_t indexCount;
		public:
			Model(Device &newDevice, Model::Builder const& builder);
			~Model();
			Model(Model const& window) = delete;
			Model &operator=(Model const) = delete;
	
			static std::unique_ptr<Model> createModelFromFile(Device &device, std::string const& filePath);
			void bind(VkCommandBuffer commandBuffer);
			void draw(VkCommandBuffer commandBuffer);
		private:
			void createVertexBuffers(std::vector<Vertex> const& vertices);
			void createIndexBuffers(std::vector<uint32_t> const& indices);
	};
}
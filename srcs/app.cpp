/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 22:42:42 by tmoragli          #+#    #+#             */
/*   Updated: 2024/03/04 15:18:12 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.hpp"
#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


namespace scop {
	App::App() {
		loadScopObjects();
	}
	App::~App() {}

	void App::run() {
		SimpleRenderSystem simpleRenderSystem{_device, _renderer.getSwapChainRenderPass()};
		while (!_window.shouldClose()) {
			glfwPollEvents();
			if (auto commandBuffer = _renderer.beginFrame()) {

				/*	Begin offscreen shadow pass
					Render shadow casting objects
					End offscreen shadow pass
				*/
				_renderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderScopObjects(commandBuffer, scopObjects);
				_renderer.endSwapChainRenderPass(commandBuffer);
				_renderer.endFrame();
			}
		}

		vkDeviceWaitIdle(_device.device());
	}

	void App::loadScopObjects()
	{
		std::vector<Model::Vertex> vertices
		{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};

		//sierpinski(4, vertices, {-1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, -1.0f});
		auto _model = std::make_shared<Model>(_device, vertices);

		auto triangle = ScopObject::createScopObject();
		triangle.model = _model;
		triangle.color = {0.1f, 0.8f, 0.1f};
		triangle.transform2D.translation.x = 0.2f;
		triangle.transform2D.scale = {2.f, 0.5f};
		triangle.transform2D.rotation = 0.25f * glm::two_pi<float>();

		scopObjects.push_back(std::move(triangle));
	}
} // scop namespace
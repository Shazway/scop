/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 22:42:42 by tmoragli          #+#    #+#             */
/*   Updated: 2024/03/10 16:53:59 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.hpp"
#include "camera.hpp"
#include "simple_render_system.hpp"
#include "keyboard_inputs.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define MAX_FRAME_TIME 150.0f
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <chrono>

namespace scop {
	App::App() {
		loadScopObjects();
	}
	App::~App() {}

	void App::run() {
		SimpleRenderSystem simpleRenderSystem{_device, _renderer.getSwapChainRenderPass()};
		Camera camera{};

		auto viewerObject = ScopObject::createScopObject();
		KeyboardInputs cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!_window.shouldClose()) {
			glfwPollEvents();
			float aspect = _renderer.getAspectRatio();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			frameTime = glm::min(frameTime, MAX_FRAME_TIME);

			cameraController.moveInPlaneXZ(_window.getGLFWwindow(), frameTime, viewerObject);
			camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

			objPosition pos{-aspect, aspect, -1, 1};
			distance perspDist{0.1f, 100.f};
			camera.setPerspectiveProjection(glm::radians(50.f), aspect, perspDist);
			if (auto commandBuffer = _renderer.beginFrame()) {
				_renderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderScopObjects(commandBuffer, scopObjects, camera);
				_renderer.endSwapChainRenderPass(commandBuffer);
				_renderer.endFrame();
			}
		}

		vkDeviceWaitIdle(_device.device());
	}

	void App::loadScopObjects()
	{
		std::shared_ptr<Model> model = Model::createModelFromFile(_device, "models/smooth_vase.obj");
		auto scopObject = ScopObject::createScopObject();
		scopObject.model = model;
		scopObject.transform.translation = {0.0f, 0.0f, 2.5f};
		scopObject.transform.scale = glm::vec3{1.5f};
		scopObjects.push_back(std::move(scopObject));
	}
} // scop namespace
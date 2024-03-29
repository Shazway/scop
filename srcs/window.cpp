/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 22:42:35 by telli             #+#    #+#             */
/*   Updated: 2024/02/02 23:18:46 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.hpp"

#include <GLFW/glfw3.h>

namespace scop {
	
	Window::Window(int w, int h, std::string name): _width{w}, _height{h}, _name{name} {
		initWindow();
	}

	Window::~Window() {
		glfwDestroyWindow(_window);
		glfwTerminate();
	}

	void Window::initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		_window = glfwCreateWindow(_width, _height, _name.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(_window, this);
		glfwSetFramebufferSizeCallback(_window, framebufferResizedCallback);
	}

	void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
		if (glfwCreateWindowSurface(instance, _window, nullptr, surface) != VK_SUCCESS)
			throw std::runtime_error("failed to create window instance");
	}

	void Window::framebufferResizedCallback(GLFWwindow *window, int width, int height) {
		auto newWindow = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));

		newWindow->framebufferResized = true;
		newWindow->_width = width;
		newWindow->_width = height;
	}
}// scop namespace
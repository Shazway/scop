/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 22:42:42 by tmoragli          #+#    #+#             */
/*   Updated: 2024/02/21 22:22:55 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "colors.hpp"

namespace scop {

	class Window {
		private:
			int	_width;
			int	_height;
			bool framebufferResized = false;
			std::string	_name;
			GLFWwindow	*_window;

		public:
			Window(int w, int h, std::string name);
			Window(Window const& window) = delete;
			~Window();
			Window &operator=(Window const) = delete;

			bool shouldClose () { return glfwWindowShouldClose(_window); }
			void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
			VkExtent2D getExtent() { return {static_cast<uint32_t>(_width), static_cast<uint32_t>(_height)}; }
			bool wasWindowResized() { return framebufferResized; }
			void resetWindowResizedFlag() { framebufferResized = false; }
		private:
			static void framebufferResizedCallback(GLFWwindow *window, int width, int height);
			void initWindow();
	};
} // scop namespace

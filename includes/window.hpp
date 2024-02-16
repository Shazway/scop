/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 22:42:42 by tmoragli          #+#    #+#             */
/*   Updated: 2024/02/04 02:49:33 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "colors.hpp"

namespace scop {

	class Window {
		private:
			const int	_width;
			const int	_height;
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
		private:
			void initWindow();
	};
} // scop namespace

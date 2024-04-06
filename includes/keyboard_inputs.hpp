/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_inputs.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 21:57:04 by tmoragli          #+#    #+#             */
/*   Updated: 2024/03/10 16:52:30 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "scop_object.hpp"
#include "window.hpp"

namespace scop {
	class KeyboardInputs {
		public: 
			struct KeyMappings {
				int moveLeft = GLFW_KEY_A;
				int moveRight = GLFW_KEY_D;
				int moveForward = GLFW_KEY_W;
				int moveBackward = GLFW_KEY_S;
				int moveUp = GLFW_KEY_LEFT_SHIFT;
				int moveDown = GLFW_KEY_SPACE;
				int lookLeft = GLFW_KEY_LEFT;
				int lookRight = GLFW_KEY_RIGHT;
				int lookUp = GLFW_KEY_DOWN;
				int lookDown = GLFW_KEY_UP;
			};
			
			void moveInPlaneXZ(GLFWwindow *window, float dt, ScopObject &scopObject);

			KeyMappings keys{};
			float moveSpeed{3.f};
			float lookSpeed{1.5f};
	};
};
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_inputs.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 22:02:01 by tmoragli          #+#    #+#             */
/*   Updated: 2024/04/06 21:15:41 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "keyboard_inputs.hpp"

namespace scop {
	void KeyboardInputs::moveInPlaneXZ(GLFWwindow *window, float dt, ScopObject &scopObject) {
		glm::vec3 rotate{0};
		if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS)
			rotate.y += 1.f;
		if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS)
			rotate.y -= 1.f;
		if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS)
			rotate.x -= 1.f;
		if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS)
			rotate.x += 1.f;

		if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
			scopObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
		
		// limit pitch values between at -85 and 85 degrees
		scopObject.transform.rotation.x = glm::clamp(scopObject.transform.rotation.x, -1.5f, 1.5f);
		scopObject.transform.rotation.y = glm::mod(scopObject.transform.rotation.y, glm::two_pi<float>());

		float yaw = scopObject.transform.rotation.y;
		const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
		const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
		const glm::vec3 upDir{0.f, -1.f, 0.f};

		glm::vec3 moveDir{0.f};

		if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS)
			moveDir += forwardDir;
		if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS)
			moveDir -= forwardDir;
		if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS)
			moveDir += rightDir;
		if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS)
			moveDir -= rightDir;
		if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS)
			moveDir -= upDir;
		if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS)
			moveDir += upDir;
		if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
			scopObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
	}
};
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 15:31:47 by tmoragli          #+#    #+#             */
/*   Updated: 2024/04/06 21:13:14 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.hpp"

#include <cassert>
#include <limits>

namespace scop {
	void Camera::setOrthographicProjection(objPosition pos, distance dist) {
		projectionMatrix = glm::mat4{1.0f};
		projectionMatrix[0][0] = 2.f / (pos.right - pos.left);
		projectionMatrix[1][1] = 2.f / (pos.bottom - pos.top);
		projectionMatrix[2][2] = 1.f / (dist.far - dist.near);
		projectionMatrix[3][0] = -(pos.right + pos.left) / (pos.right - pos.left);
		projectionMatrix[3][1] = -(pos.bottom + pos.top) / (pos.bottom - pos.top);
		projectionMatrix[3][2] = -dist.near / (dist.far - dist.near);
	}

	void Camera::setPerspectiveProjection(float fovy, float aspect, distance dist) {
		assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
		const float tanHalfFovy = tan(fovy / 2.f);
		projectionMatrix = glm::mat4{0.0f};
		projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
		projectionMatrix[1][1] = 1.f / (tanHalfFovy);
		projectionMatrix[2][2] = dist.far / (dist.far - dist.near);
		projectionMatrix[2][3] = 1.f;
		projectionMatrix[3][2] = -(dist.far * dist.near) / (dist.far - dist.near);
	}

	void Camera::setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up) {
		const glm::vec3 w{glm::normalize(direction)};
		const glm::vec3 u{glm::normalize(glm::cross(w, up))};
		const glm::vec3 v{glm::cross(w, u)};

		viewMatrix = glm::mat4{1.f};
		viewMatrix[0][0] = u.x;
		viewMatrix[1][0] = u.y;
		viewMatrix[2][0] = u.z;
		viewMatrix[0][1] = v.x;
		viewMatrix[1][1] = v.y;
		viewMatrix[2][1] = v.z;
		viewMatrix[0][2] = w.x;
		viewMatrix[1][2] = w.y;
		viewMatrix[2][2] = w.z;
		viewMatrix[3][0] = -glm::dot(u, position);
		viewMatrix[3][1] = -glm::dot(v, position);
		viewMatrix[3][2] = -glm::dot(w, position);
	}

	void Camera::setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
		setViewDirection(position, target - position, up);
	}

	void Camera::setViewYXZ(glm::vec3 position, glm::vec3 rotation) {
		const float c3 = glm::cos(rotation.z);
		const float s3 = glm::sin(rotation.z);
		const float c2 = glm::cos(rotation.x);
		const float s2 = glm::sin(rotation.x);
		const float c1 = glm::cos(rotation.y);
		const float s1 = glm::sin(rotation.y);
		const glm::vec3 u{(c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1)};
		const glm::vec3 v{(c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3)};
		const glm::vec3 w{(c2 * s1), (-s2), (c1 * c2)};
		viewMatrix = glm::mat4{1.f};
		viewMatrix[0][0] = u.x;
		viewMatrix[1][0] = u.y;
		viewMatrix[2][0] = u.z;
		viewMatrix[0][1] = v.x;
		viewMatrix[1][1] = v.y;
		viewMatrix[2][1] = v.z;
		viewMatrix[0][2] = w.x;
		viewMatrix[1][2] = w.y;
		viewMatrix[2][2] = w.z;
		viewMatrix[3][0] = -glm::dot(u, position);
		viewMatrix[3][1] = -glm::dot(v, position);
		viewMatrix[3][2] = -glm::dot(w, position);
	}
};
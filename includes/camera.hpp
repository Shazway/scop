/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 15:25:01 by tmoragli          #+#    #+#             */
/*   Updated: 2024/03/10 16:46:29 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

struct distance {
	float near;
	float far;
};

struct objPosition {
	float left;
	float right;
	float top;
	float bottom;
};



namespace scop {
	class Camera {
		private:
			glm::mat4 projectionMatrix{1.f};
			glm::mat4 viewMatrix{1.f};
		public:
			void setOrthographicProjection(objPosition pos, distance dist);
			void setPerspectiveProjection(float fovy, float aspect, distance dist);
			void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = {0.f, -1.f, 0.f});
			void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = {0.f, -1.f, 0.f});
			void setViewYXZ(glm::vec3 position, glm::vec3 rotation);
			const glm::mat4& getProjection() const { return projectionMatrix; }
			const glm::mat4& getView() const { return viewMatrix; }
	};
};
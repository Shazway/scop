/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop_object.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 13:57:52 by tmoragli          #+#    #+#             */
/*   Updated: 2024/03/09 00:14:57 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "model.hpp"
#include <memory>
#include <glm/gtc/matrix_transform.hpp>

namespace scop {
	struct TransformComponent {
		glm::vec3 translation{};
		glm::vec3 scale{1.f, 1.f, 1.f};
		glm::vec3 rotation;

		glm::mat4 mat4() {
			auto transform = glm::translate(glm::mat4{1.0f}, translation);
			// Matrix corresponds to translate * Ry * Rx * Rz * scale transformation
			// Rotation convention uses tait-bryan angles with axis order Y(1), X(2), Z(3)
			transform = glm::rotate(transform, rotation.y, {0.0f, 1.0f, 0.0f});
			transform = glm::rotate(transform, rotation.x, {1.0f, 0.0f, 0.0f});
			transform = glm::rotate(transform, rotation.z, {0.0f, 0.0f, 1.0f});
			transform = glm::scale(transform, scale);
			return transform;
		}
	};

	class ScopObject {
		public:
			using id_t = unsigned int;

			std::shared_ptr<Model> model{};
			glm::vec3 color{};
			TransformComponent transform{};
		private:
			ScopObject(id_t objId): id{objId} {};
			id_t id;
		public:
			static ScopObject createScopObject() {
				static id_t currentId = 0;
				return ScopObject{currentId++};
			}
			id_t getId() { return id; };

			ScopObject(ScopObject const&) = delete;
			ScopObject(ScopObject &&) = default;
			ScopObject &operator=(ScopObject const&) = delete;
			ScopObject &operator=(ScopObject &&) = default;
	};
}
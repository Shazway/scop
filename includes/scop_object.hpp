/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop_object.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 13:57:52 by tmoragli          #+#    #+#             */
/*   Updated: 2024/03/04 14:34:07 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "model.hpp"
#include <memory>


namespace scop {
	struct Transform2dComponent {
		glm::vec2 translation{};
		glm::vec2 scale{1.f, 1.f};
		float rotation;

		glm::mat2 mat2() {
			const float sin = glm::sin(rotation);
			const float cos = glm::cos(rotation);
			glm::mat2 rotMatrix{{cos, sin}, {-sin, cos}};
			
			glm::mat2 scaleMat{{scale.x, 0.0f}, {0.0f, scale.y}};
			return rotMatrix * scaleMat;
		}
	};

	class ScopObject {
		public:
			using id_t = unsigned int;

			std::shared_ptr<Model> model{};
			glm::vec3 color{};
			Transform2dComponent transform2D{};
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
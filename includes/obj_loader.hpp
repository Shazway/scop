/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_loader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 19:19:31 by tmoragli          #+#    #+#             */
/*   Updated: 2024/06/26 20:39:32 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJ_LOADER_HPP
#define OBJ_LOADER_HPP

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>


#include "shapes.hpp"
#include "materials.hpp"

namespace scop {
	struct obj_data {
		std::vector<float> vertices;  // 'v'(xyz)

		std::vector<float> normals;         // 'vn'
		std::vector<float> texcoords;       // 'vt'(uv)
		std::vector<float> texcoord_ws;  // 'vt'(w)

		std::vector<float> colors;       // extension: vertex colors

		obj_data() {}
		const std::vector<float> &GetVertices() const { return vertices; }
	};

	struct loaded_obj {
		obj_data data;
		std::vector<shape_t> shapes;
		std::vector<material_t> materials;
	};

	class ObjLoader {
		public:
			ObjLoader();
			~ObjLoader();
			bool loadModel(loaded_obj &obj, std::string &warn, std::string &error);
	};
};

#endif
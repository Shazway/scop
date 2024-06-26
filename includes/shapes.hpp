/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 20:19:42 by tmoragli          #+#    #+#             */
/*   Updated: 2024/06/26 20:35:56 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <iostream>

namespace scop {
	struct index_t {
		int vertex_index;
		int normal_index;
		int texcoord_index;
	};


	struct tag_t {
		std::string name;

		std::vector<int> intValues;
		std::vector<float> floatValues;
		std::vector<std::string> stringValues;
	};

	struct mesh_t {
		std::vector<index_t> indices;
		std::vector<unsigned int>
			num_face_vertices;          // The number of vertices per
										// face. 3 = triangle, 4 = quad, ...
		std::vector<int> material_ids;  // per-face material ID
		std::vector<unsigned int> smoothing_group_ids;  // per-face smoothing group
														// ID(0 = off. positive value
														// = group id)
		std::vector<tag_t> tags;                        // SubD tag
	};

	struct lines_t {
		// Linear flattened indices.
		std::vector<index_t> indices;        // indices for vertices(poly lines)
		std::vector<int> num_line_vertices;  // The number of vertices per line.
	};

	struct points_t {
		std::vector<index_t> indices;  // indices for points
	};

	struct shape_t {
		std::string name;
		mesh_t mesh;
		lines_t lines;
		points_t points;
	};
};
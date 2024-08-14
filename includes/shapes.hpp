/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 20:19:42 by tmoragli          #+#    #+#             */
/*   Updated: 2024/07/25 02:21:07 by tmoragli         ###   ########.fr       */
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
	
	// Internal data structure for line representation
	struct __line_t {
		// l v1/vt1 v2/vt2 ...
		// In the specification, line primitrive does not have normal index.
		std::vector<vertex_index_t> vertex_indices;
	};

	// Internal data structure for face representation
	// index + smoothing group.
	struct face_t {
		unsigned int
			smoothing_group_id;  // smoothing group id. 0 = smoothing groupd is off.
		int pad_;
		std::vector<vertex_index_t> vertex_indices;  // face vertex indices.

		face_t() : smoothing_group_id(0), pad_(0) {}
	};

	// Internal data structure for points representation
	struct __points_t {
		// p v1 v2 ...
		// In the specification, point primitrive does not have normal index and
		// texture coord index
		std::vector<vertex_index_t> vertex_indices;
	};
};
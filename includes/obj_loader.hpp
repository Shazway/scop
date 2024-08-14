/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_loader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 19:19:31 by tmoragli          #+#    #+#             */
/*   Updated: 2024/07/27 20:25:45 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJ_LOADER_HPP
#define OBJ_LOADER_HPP

# define NO_SMOOTHING 0
# define SMOOTHING 1

#define IS_SPACE(x) (((x) == ' ') || ((x) == '\t'))
#define IS_DIGIT(x) \
  (static_cast<unsigned int>((x) - '0') < static_cast<unsigned int>(10))
#define IS_NEW_LINE(x) (((x) == '\r') || ((x) == '\n') || ((x) == '\0'))

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include <string.h>

#include "shapes.hpp"
#include "materials.hpp"
#include "parsing_utils.hpp"

#include <bits/stl_iterator_base_funcs.h>
#include <bits/functexcept.h>
#include <bits/concept_check.h>

namespace scop {
	struct vertex_index_t {
		int v_idx;
		int vt_idx;
		int vn_idx;
		public:
			vertex_index_t() : v_idx(-1), vt_idx(-1), vn_idx(-1) {}
			explicit vertex_index_t(int idx) : v_idx(idx), vt_idx(idx), vn_idx(idx) {}
			vertex_index_t(int vidx, int vtidx, int vnidx) : v_idx(vidx), vt_idx(vtidx), vn_idx(vnidx) {}
	};

	struct PrimGroup {
		std::vector<face_t> faceGroup;
		std::vector<__line_t> lineGroup;
		std::vector<__points_t> pointsGroup;

		void clear() {
			faceGroup.clear();
			lineGroup.clear();
			pointsGroup.clear();
		}

		bool IsEmpty() const {
			return faceGroup.empty() && lineGroup.empty() && pointsGroup.empty();
		}
	};

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
		std::string obj_path;
		std::string mtl_path;
		std::vector<shape_t> shapes;
		std::vector<material_t> materials;
	};

	class ObjLoader {
		public:
			ObjLoader() {};
			~ObjLoader() {};

			void clearObj(loaded_obj &obj)
			{
				obj.data.vertices.clear();
				obj.data.normals.clear();
				obj.data.texcoords.clear();
				obj.data.texcoord_ws.clear();
				obj.data.colors.clear();
				obj.materials.clear();
				obj.shapes.clear();
			}

			bool loadModel(loaded_obj &obj, std::string &warn, std::string *error)
			{
				// Make sure we are working with a fresh object to avoid conflicts
				clearObj(obj);

				std::stringstream errss;

				std::ifstream ifs(obj.obj_path);
				if (!ifs)
				{
					errss << "Cannot open file [" << obj.obj_path << "]\n";
					if (error)
						(*error) = errss.str();
					return false;
				}
				std::vector<float> v;
				std::vector<float> vn;
				std::vector<float> vt;
				std::vector<float> vc;
				std::vector<tag_t> tags;
				PrimGroup prim_group;
				std::string name;

				// Smoothing group id
				unsigned int current_smoothing_id = NO_SMOOTHING; 

				int greatest_v_idx = -1;
				int greatest_vn_idx = -1;
				int greatest_vt_idx = -1;

				shape_t shape;
				
				// Check if all 'v' line has color
				bool found_all_colors = true;
				 
				size_t line_num = 0;
				std::string line;

				if (ifs.peek() == std::ifstream::traits_type::eof())
				{
					errss << "File [" << obj.obj_path << "] is empty\n";
					if (error)
						(*error) = errss.str();
					return false;
				}
				while (std::getline(ifs, line))
				{
					line_num++;
					std::istringstream iss(line);
					std::string word;
					while (iss >> word)
					{
						// Remove terminating \n or \r
						if (line.size() && line[line.size() - 1] == '\n')
        					line.erase(line.size() - 1);
						if (line.size() && line[line.size() - 1] == '\r')
        					line.erase(line.size() - 1);
						
						// Ignore empty lines
						if (line.empty())
							continue ;

						// Skip spaces
						const char *token = line.c_str();
    					token += strspn(token, " \t\r");
						assert(token);

						// Ignore empty line (or only made of whitespaces)
						if (token[0] == '\0') continue;
						// Commented line
						if (token[0] == '#') continue;

						// TODO (tmoragli)
						// Add material loading
						//loadMaterials(...);

						if (token[0] == 'v' && IS_SPACE(token[1]))
						{
							token += 2;
							float x, y, z;
							float r, g, b;

							int num_components = parseVertexWithColor(&x, &y, &z, &r, &g, &b, &token);
							found_all_colors &= (num_components == 6);

							v.push_back(x);
							v.push_back(y);
							v.push_back(z);

							if ((num_components == 6)) {
								vc.push_back(r);
								vc.push_back(g);
								vc.push_back(b);
							}

							continue;
						}
						
					}
				}
			}
	};
};

#endif
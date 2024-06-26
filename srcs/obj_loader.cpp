/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_loader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 20:30:40 by tmoragli          #+#    #+#             */
/*   Updated: 2024/06/26 20:51:14 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj_loader.hpp"

namespace scop {
	bool loadModel(loaded_obj *obj, const std::string filename, std::string *warn, std::string *error) {
		obj->materials.clear();
		obj->shapes.clear();
		obj->data.colors.clear();
		obj->data.normals.clear();
		obj->data.texcoord_ws.clear();
		obj->data.texcoords.clear();
		obj->data.vertices.clear();

		std::stringstream errss;

		std::ifstream ifs(filename);
		if (!ifs)
		{
			errss << "Cannot open file [" << filename << "]\n";
			if (error) {
			(*error) = errss.str();
			}
			return false;
		}
	}
};
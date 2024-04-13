/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry_parser.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 21:59:01 by tmoragli          #+#    #+#             */
/*   Updated: 2024/04/12 00:03:24 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <glm/glm.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <string>
#include <vector>
#include <optional>
#include "model.hpp"

namespace fs = std::filesystem;

namespace scop {

	struct Context {
		Context(std::string const& path) {
			obj_filename = fs::path(path);

			if (!exists(obj_filename) || !is_regular_file(obj_filename))
				std::cerr << "error: " << obj_filename.string() << ": expecting an existing regular file" << std::endl;
		}

		fs::path obj_filename;
	};


	struct Obj {
		Obj() = default;
		Obj(std::string obj_name): name(obj_name) { }

		struct FaceIndices {
			uint32_t				vertex{};
			std::optional<uint32_t> texture;
			std::optional<uint32_t> normal;
		};

		struct Group {
			std::string								name;
			std::vector<std::array<FaceIndices, 3>> faces;
			std::string								material;
		};

		std::optional<std::string> name;

		std::string				   material;
		std::vector<glm::vec3>	   vertices;
		std::vector<glm::vec2>	   textures;
		std::vector<glm::vec3>	   normals;
		std::vector<Group>		   groups;
	};

	struct File {
		std::shared_ptr<Obj> obj;

		std::vector<fs::path>			  mtllibs;
	};

	struct Material {
		float	  Ns;
		glm::vec3 Ka;
		glm::vec3 Kd;
		glm::vec3 Ks;
	};

	struct LoadedObj {
		bool loadedState = false;
		std::unordered_map<std::string, Material> materials;
		std::unique_ptr<File> object;

		static LoadedObj parse_obj_file(std::string const& path) {
			Context		  ctx(path);
			std::ifstream obj(ctx.obj_filename);
			LoadedObj	loaded_obj;

			if (!obj)
			{
				std::cerr << "error: " << ctx.obj_filename << ": couldn't open obj file for reading" << std::endl;
				return loaded_obj;
			}
			loaded_obj.object = load(obj);
			loaded_obj.materials = load_materials(ctx.obj_filename, loaded_obj.object->mtllibs);
			loaded_obj.loadedState = true;
			return loaded_obj;
		}
	};

	std::istream &operator>>(std::istream &ifs, glm::vec3 &target) {
		ifs >> target.x >> target.y >> target.z;
		return ifs;
	}

	std::istream &operator>>(std::istream &ifs, glm::vec2 &target) {
		ifs >> target.x >> target.y;
		return ifs;
	}
};

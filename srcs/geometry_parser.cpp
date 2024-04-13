/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry_parser.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 14:09:19 by tmoragli          #+#    #+#             */
/*   Updated: 2024/04/12 00:02:57 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "geometry_parser.hpp"

namespace fs = std::filesystem;

namespace scop {
	std::vector<std::string> split(const std::string &str) {
		std::stringstream		 ss(str);
		std::vector<std::string> ret;

		std::string				 word;
		while (std::getline(ss, word, '/')) {
			ret.push_back(word);
		}

		return ret;
	}

	std::unique_ptr<File> load(std::ifstream &obj) {
		auto		ret		= std::make_unique<File>();
		auto		cur_obj = std::make_shared<Obj>();

		std::string line;
		auto		grp_it = cur_obj->groups.end();
		while (std::getline(obj, line)) {
			std::istringstream iss(line);

			std::string		   id;
			iss >> id;

			if (id == "mtllib")
				iss >> ret->mtllibs.emplace_back();
			else if (id == "v")
				iss >> cur_obj->vertices.emplace_back();
			else if (id == "vt")
				iss >> cur_obj->textures.emplace_back();
			else if (id == "vn")
				iss >> cur_obj->normals.emplace_back();
			else if (id == "g")
			{
				std::string name;
				iss >> name;

				Obj::Group grp{};
				grp.name = name;
				grp_it	 = cur_obj->groups.insert(cur_obj->groups.begin(), grp);
			}
			else if (id == "usemtl")
			{
				if (grp_it == cur_obj->groups.end())
					throw std::runtime_error("expecting materials only within groups");

				iss >> grp_it->material;
			}
			else if (id == "f")
			{
				if (grp_it == cur_obj->groups.end())
					throw std::runtime_error("expecting faces only within groups");

				auto					  &face = grp_it->faces.emplace_back();
				std::array<std::string, 3> vertices;

				iss >> vertices[0] >> vertices[1] >> vertices[2];
				for (size_t i = 0; i < 3; i++) {
					std::vector arr = split(vertices[i]);

					face[i].vertex	= std::stoi(arr[0]);

					if (arr.size() > 1 && !arr[1].empty())
						face[i].texture = std::stoi(arr[1]);
					if (arr.size() > 2 && !arr[2].empty())
						face[i].normal = std::stoi(arr[2]);
				}
			}
		}

		return ret;
	}

	std::unordered_map<std::string, Material> load_materials(const fs::path &obj, const std::vector<fs::path> &mtllib) {
		fs::path								  dir = absolute(obj).parent_path();

		std::unordered_map<std::string, Material> ret;
		auto									  cur = ret.end();
		for (const auto &lib : mtllib)
		{
			std::ifstream ifs;

			if (lib.is_absolute())
				ifs.open(lib);
			else
				ifs.open(dir / lib);

			std::string line;
			while (std::getline(ifs, line)) {
				std::istringstream iss(line);
				std::string		   id;
				iss >> id;

				if (id == "newmtl")
				{
					std::string name;
					iss >> name;
					auto [it, is_inserted] = ret.insert_or_assign(name, Material{});
					cur					   = it;

					std::cout << (is_inserted ? "Inserted" : "Replaced") << " material with name " << name;
				}
				else if (id == "Ka")
				{
					if (cur == ret.end())
						throw std::runtime_error("can't update non existent material, please declare one before anything else");
					iss >> cur->second.Ka;
				}
				else if (id == "Ks")
				{
					if (cur == ret.end())
						throw std::runtime_error("can't update non existent material, please declare one before anything else");
					iss >> cur->second.Ks;
				}
				else if (id == "Kd")
				{
					if (cur == ret.end())
						throw std::runtime_error("can't update non existent material, please declare one before anything else");
					iss >> cur->second.Kd;
				}
				else if (id == "Ns")
				{
					if (cur == ret.end())
						throw std::runtime_error("can't update non existent material, please declare one before anything else");
					iss >> cur->second.Ns;
				}
			}
		}

		return ret;
	}
};

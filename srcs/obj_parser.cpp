/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 00:05:28 by tmoragli          #+#    #+#             */
/*   Updated: 2024/04/18 00:21:49 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <glm/glm.hpp>

struct Vertex {
	glm::vec3 position{};
	glm::vec3 color{};
	glm::vec3 normal{};
	glm::vec2 uv{};

	bool operator==(Vertex const& other) const {
		return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
	}
};

struct Builder {
	std::vector<Vertex> vertices {};
	std::vector<uint32_t> indices {}; 

    void processFace(const std::string& faceVertex) {
        std::istringstream viss(faceVertex);
        std::string vertexIndex;
        std::getline(viss, vertexIndex, '/');

        int idx = std::stoi(vertexIndex) - 1;
        indices.push_back(idx);
    }

	void loadModel(std::string const& filePath) {
		std::ifstream file(filePath);
		if (!file.is_open()) {
			std::cerr << "Error opening file: " << filePath << std::endl;
			return;
		}

		std::string line;
		while (std::getline(file, line)) {
			std::istringstream iss(line);
			std::string token;
			iss >> token;
			if (token == "v") {
				Vertex vertex;
				iss >> vertex.position.x >> vertex.position.y >> vertex.position.z;
				vertex.color = {1.f, 1.f, 1.f};
				vertices.push_back(vertex);
			} else if (token == "vn") {
				glm::vec3 normal;
				iss >> normal.x >> normal.y >> normal.z;
				vertices.back().normal = normal;
			} else if (token == "vt") {
				glm::vec2 uv;
				iss >> uv.x >> uv.y;
				vertices.back().uv = uv;
			} else if (token == "f") {
				std::string vertex1, vertex2, vertex3;
				iss >> vertex1 >> vertex2 >> vertex3;
			}
            else if (token == "f") {
                std::string vertex1, vertex2, vertex3;
                iss >> vertex1 >> vertex2 >> vertex3;
                processFace(vertex1);
                processFace(vertex2);
                processFace(vertex3);
            }
        }
        file.close();
    }
};

int main() {
	Builder builder;
	builder.loadModel("models/colored_cube.obj");

	// Do something with the loaded data, like printing it
	for (const auto& vertex : builder.vertices) {
		std::cout << "Position: (" << vertex.position.x << ", " << vertex.position.y << ", " << vertex.position.z << ")" << std::endl;
		std::cout << "Normal: (" << vertex.normal.x << ", " << vertex.normal.y << ", " << vertex.normal.z << ")" << std::endl;
		std::cout << "UV: (" << vertex.uv.x << ", " << vertex.uv.y << ")" << std::endl;
	}
	return 0;
}

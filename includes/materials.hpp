/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   materials.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 20:17:31 by tmoragli          #+#    #+#             */
/*   Updated: 2024/06/26 20:28:20 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIALS_HPP
#define MATERIALS_HPP

#include <string>
#include <map>

namespace scop {

	typedef enum {
		TEXTURE_TYPE_NONE,
		TEXTURE_TYPE_SPHERE,
		TEXTURE_TYPE_CUBE_TOP,
		TEXTURE_TYPE_CUBE_BOTTOM,
		TEXTURE_TYPE_CUBE_FRONT,
		TEXTURE_TYPE_CUBE_BACK,
		TEXTURE_TYPE_CUBE_LEFT,
		TEXTURE_TYPE_CUBE_RIGHT
	} texture_type_t;

	struct texture_option_t {
		texture_type_t type;      // -type (default TEXTURE_TYPE_NONE)
		float sharpness;         // -boost (default 1.0?)
		float brightness;        // base_value in -mm option (default 0)
		float contrast;          // gain_value in -mm option (default 1)
		float origin_offset[3];  // -o u [v [w]] (default 0 0 0)
		float scale[3];          // -s u [v [w]] (default 1 1 1)
		float turbulence[3];     // -t u [v [w]] (default 0 0 0)
		int texture_resolution;   // -texres resolution (No default value in the spec.
									// We'll use -1)
		bool clamp;               // -clamp (default false)
		char imfchan;  // -imfchan (the default for bump is 'l' and for decal is 'm')
		bool blendu;   // -blendu (default on)
		bool blendv;   // -blendv (default on)
		float bump_multiplier;  // -bm (for bump maps only, default 1.0)

		// extension
		std::string colorspace;  // Explicitly specify color space of stored texel
								// value. Usually `sRGB` or `linear` (default empty).
	};

	struct material_t {
		std::string name;

		float ambient[3];
		float diffuse[3];
		float specular[3];
		float transmittance[3];
		float emission[3];
		float shininess;
		float ior;       // index of refraction
		float dissolve;  // 1 == opaque; 0 == fully transparent
		int illum;

		int dummy;  // Suppress padding warning.

		std::string ambient_texname;   // map_Ka. For ambient or ambient occlusion.
		std::string diffuse_texname;   // map_Kd
		std::string specular_texname;  // map_Ks
		std::string specular_highlight_texname;  // map_Ns
		std::string bump_texname;                // map_bump, map_Bump, bump
		std::string displacement_texname;        // disp
		std::string alpha_texname;               // map_d
		std::string reflection_texname;          // refl

		texture_option_t ambient_texopt;
		texture_option_t diffuse_texopt;
		texture_option_t specular_texopt;
		texture_option_t specular_highlight_texopt;
		texture_option_t bump_texopt;
		texture_option_t displacement_texopt;
		texture_option_t alpha_texopt;
		texture_option_t reflection_texopt;

		float roughness;            // [0, 1] default 0
		float metallic;             // [0, 1] default 0
		float sheen;                // [0, 1] default 0
		float clearcoat_thickness;  // [0, 1] default 0
		float clearcoat_roughness;  // [0, 1] default 0
		float anisotropy;           // aniso. [0, 1] default 0
		float anisotropy_rotation;  // anisor. [0, 1] default 0
		float pad0;
		std::string roughness_texname;  // map_Pr
		std::string metallic_texname;   // map_Pm
		std::string sheen_texname;      // map_Ps
		std::string emissive_texname;   // map_Ke
		std::string normal_texname;     // norm. For normal mapping.

		texture_option_t roughness_texopt;
		texture_option_t metallic_texopt;
		texture_option_t sheen_texopt;
		texture_option_t emissive_texopt;
		texture_option_t normal_texopt;

		int pad2;

		std::map<std::string, std::string> unknown_parameter;
	};
};

#endif
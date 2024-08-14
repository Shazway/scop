/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 20:23:53 by tmoragli          #+#    #+#             */
/*   Updated: 2024/07/27 21:39:59 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_UTILS_HPP
#define PARSING_UTILS_HPP

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include <string.h>


namespace scop {
	static bool tryParseDouble(const char *s, const char *s_end, double *result) {
		if (s >= s_end)
			return false;
	}

	static inline float parseFloat(const char **token, double default_value = 0.0) {
		(*token) += strspn((*token), " \t");
		const char *end = (*token) + strcspn((*token), " \t\r");
		double val = default_value;
		tryParseDouble((*token), end, &val);
		float f = static_cast<float>(val);
		(*token) = end;
		return f;
	}

	static inline bool parseFloat(const char **token, float *out) {
		(*token) += strspn((*token), " \t");
		const char *end = (*token) + strcspn((*token), " \t\r");
		double val;
		bool ret = tryParseDouble((*token), end, &val);
		if (ret)
		{
			float f = static_cast<float>(val);
			(*out) = f;
		}
		(*token) = end;
		return ret;
	}

	static inline int parseVertexWithColor(float *x, float *y, float *z,
										float *r, float *g, float *b,
										const char **token,
										const double default_x = 0.0,
										const double default_y = 0.0,
										const double default_z = 0.0) {
		(*x) = parseFloat(token, default_x);
		(*y) = parseFloat(token, default_y);
		(*z) = parseFloat(token, default_z);

		// - 4 components(x, y, z, w) ot 6 components
		bool has_r = parseFloat(token, r);

		if (!has_r) {
			(*r) = (*g) = (*b) = 1.0;
			return 3;
		}

		bool has_g = parseFloat(token, g);

		if (!has_g) {
			(*g) = (*b) = 1.0;
			return 4;
		}

		bool has_b = parseFloat(token, b);

		if (!has_b) {
			(*r) = (*g) = (*b) = 1.0;
			return 3;  // treated as xyz
		}

		return 6;
	}

};


#endif
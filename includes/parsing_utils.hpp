/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 20:23:53 by tmoragli          #+#    #+#             */
/*   Updated: 2024/08/15 15:13:04 by tmoragli         ###   ########.fr       */
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
#include <cmath>

#define FAIL false
#define SUCCESS true
#define INT_MAX 2147483647
#define INT_MIN -2147483648

namespace scop {

	// Make index zero-base, and also support relative index.
	static inline bool fixIndex(int idx, int n, int *ret, bool allow_zero) {
		if (!ret)
			return false;

		if (idx > 0)
		{
			(*ret) = idx - 1;
			return true;
		}

		if (idx == 0)
		{
			(*ret) = idx - 1;
			return allow_zero;
		}

		if (idx < 0)
		{
			(*ret) = n + idx;  // negative value = relative
			if ((*ret) < 0)
				return false;  // invalid relative index
			return true;
		}

		return false;  // never reach here.
	}

	// Parse triples with index offsets: i, i/j/k, i//k, i/j
	static bool parseTriple(const char **token, int vsize, int vnsize, int vtsize,
							vertex_index_t *ret) {
		if (!ret)
			return false;

		vertex_index_t vi(-1);

		if (!fixIndex(atoi((*token)), vsize, &vi.v_idx, false))
			return false;

		(*token) += strcspn((*token), "/ \t\r");
		if ((*token)[0] != '/')
		{
			(*ret) = vi;
			return true;
		}
		(*token)++;

		// i//k
		if ((*token)[0] == '/')
		{
			(*token)++;
			if (!fixIndex(atoi((*token)), vnsize, &vi.vn_idx, true))
				return false;
			(*token) += strcspn((*token), "/ \t\r");
			(*ret) = vi;
			return true;
		}

		// i/j/k or i/j
		if (!fixIndex(atoi((*token)), vtsize, &vi.vt_idx, true))
			return false;

		(*token) += strcspn((*token), "/ \t\r");
		if ((*token)[0] != '/')
		{
			(*ret) = vi;
			return true;
		}

		// i/j/k
		(*token)++;  // skip '/'
		if (!fixIndex(atoi((*token)), vnsize, &vi.vn_idx, true))
			return false;

		(*token) += strcspn((*token), "/ \t\r");
		(*ret) = vi;
		return true;
	}


	static bool parseDouble(const char *s, const char *s_end, double *result)
	{
		if (s >= s_end)
			return FAIL;
		double mantissa = 0.0;
		// This exponent is base 2 rather than 10.
		// However the exponent we parse is supposed to be one of ten,
		// thus we must take care to convert the exponent/and or the
		// mantissa to a * 2^E, where a is the mantissa and E is the
		// exponent.
		// To get the final double we will use ldexp, it requires the
		// exponent to be in base 2.
		int exponent = 0;

		// NOTE: THESE MUST BE DECLARED HERE SINCE WE ARE NOT ALLOWED
		// TO JUMP OVER DEFINITIONS.
		char sign = '+';
		char exp_sign = '+';
		char const *curr = s;

		// How many characters were read in a loop.
		int read = 0;
		// Tells whether a loop terminated due to reaching s_end.
		bool end_not_reached = false;
		bool leading_decimal_dots = false;

		// Find out the sign
		if (*curr == '+' || *curr == '-')
		{
			sign = *curr;
			curr++;
			if ((curr != s_end) && (*curr == '.'))
			{
				// accept. Somethig like `+.7e+2`, `-.5234`
				leading_decimal_dots = true;
			}
		}
		else if (*curr == '.')
		{
			// accept. Somethig like `.7e+2`, `.5234`
			leading_decimal_dots = true;
		}
		else if (!IS_DIGIT(*curr))
			return FAIL;

		// Read the integer part.
		end_not_reached = (curr != s_end);
		if (!leading_decimal_dots)
		{
			while (end_not_reached && IS_DIGIT(*curr))
			{
				mantissa *= 10;
				// Conversion from char to integer digit
				mantissa += static_cast<int>(*curr - '0');
				curr++;
				read++;
				end_not_reached = (curr != s_end);
			}

			// Make sure we actually read some digits.
			if (!read)
				return FAIL;
		}

		// Allow numbers of form "#", "###" etc.
		if (!end_not_reached)
			goto assemble;

		// Reading the decimal part.
		if (*curr == '.')
		{
			curr++;
			read = 1;
			end_not_reached = (curr != s_end);
			while (end_not_reached && IS_DIGIT(*curr))
			{
				static const double pow_tab[] = {1.0, 0.1, 0.01, 0.001, 0.0001, 0.00001, 0.000001, 0.0000001};
				const int lut_entries = sizeof pow_tab / sizeof pow_tab[0];

				// Note: No use of powf here, it will murder precision.
				mantissa += static_cast<int>(*curr - '0') *
				// Avoid computational power up to last index of pow_tab, past that, uses pow for 0.00000001 and beyond
							(read < lut_entries ? pow_tab[read] : std::pow(10.0, -read));
				read++;
				curr++;
				end_not_reached = (curr != s_end);
			}
		}
		else if ((*curr != 'e' && *curr != 'E') || !end_not_reached)
			goto assemble;

		// Read the exponent part.
		if (*curr == 'e' || *curr == 'E')
		{
			curr++;
			// Figure out if a sign is present and if it is.
			end_not_reached = (curr != s_end);
			if (end_not_reached && (*curr == '+' || *curr == '-'))
			{
				exp_sign = *curr;
				curr++;
			}
			else if (!IS_DIGIT(*curr))
			{ 
				return FAIL;
			}
			read = 0;
			end_not_reached = (curr != s_end);
			while (end_not_reached && IS_DIGIT(*curr))
			{
				// Use hardcoded int max value
				if (exponent > (INT_MAX / 10))
					return FAIL; // Integer overflow
				exponent *= 10;
				exponent += static_cast<int>(*curr - '0');
				curr++;
				read++;
				end_not_reached = (curr != s_end);
			}
			exponent *= (exp_sign == '+' ? 1 : -1);
			if (read == 0)
				return FAIL;
		}

		assemble:
		int sign_n = (sign == '+' ? 1 : -1); // Determines positive and negative
		// The multiplier
		double exponent_n = (exponent ? std::ldexp(mantissa * std::pow(5.0, exponent), exponent) : mantissa);

		*result = sign_n * exponent_n;
		return SUCCESS;
	}

	static inline float parseFloat(const char **token, double default_value = 0.0) {
		(*token) += strspn((*token), " \t");
		const char *end = (*token) + strcspn((*token), " \t\r");
		double val = default_value;
		parseDouble((*token), end, &val);
		float f = static_cast<float>(val);
		(*token) = end;
		return f;
	}

	static inline bool parseFloat(const char **token, float *out) {
		(*token) += strspn((*token), " \t");
		const char *end = (*token) + strcspn((*token), " \t\r");
		double val;
		bool ret = parseDouble((*token), end, &val);
		if (ret)
		{
			float f = static_cast<float>(val);
			(*out) = f;
		}
		(*token) = end;
		return ret;
	}

	static inline void parseFloat2(float *x, float *y, const char **token,
								const double default_x = 0.0,
								const double default_y = 0.0) {
		(*x) = parseFloat(token, default_x);
		(*y) = parseFloat(token, default_y);
	}

	static inline void parseFloat3(float *x, float *y, float *z,
								const char **token, const double default_x = 0.0,
								const double default_y = 0.0,
								const double default_z = 0.0) {
		(*x) = parseFloat(token, default_x);
		(*y) = parseFloat(token, default_y);
		(*z) = parseFloat(token, default_z);
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
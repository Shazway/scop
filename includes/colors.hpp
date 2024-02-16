/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 23:16:58 by tmoragli          #+#    #+#             */
/*   Updated: 2024/02/02 23:22:16 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>

#define GGLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h> 

//STD includes
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <cstring>
#include <set>
#include <unordered_set>


#define GLFW_INCLUDE_VULKAN

// Printing colors for debugging

#define C_BLACK "\e[1;30m"
#define C_RED "\e[1;31m"
#define C_GREEN "\e[1;32m"
#define C_YELLOW "\e[1;33m"
#define C_BLUE "\e[1;34m"
#define C_MAGENTA "\e[1;35m"
#define C_CYAN "\e[1;36m"
#define C_WHITE "\e[1;37m"
#define BLINK_RED "\x1B[5;31m"
#define BLINK_BLUE "\x1B[5;34m"
#define BLINK_CYAN "\x1B[5;36m"
#define C_END "\e[0m"

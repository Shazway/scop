/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 23:03:56 by tmoragli          #+#    #+#             */
/*   Updated: 2024/03/09 14:57:51 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_HPP
#define APP_HPP

#include "window.hpp"
#include "device.hpp"
#include "renderer.hpp"
#include "model.hpp"
#include "scop_object.hpp"
#include <memory>
#include <vector>

#define WIDTH 760
#define HEIGHT 760

namespace scop {

	class App {
		private:
			Window	_window{WIDTH, HEIGHT, "Scop"};
			Device	_device{_window};
			Renderer _renderer{_window, _device};
			std::vector<ScopObject>	scopObjects;
		public:
			App();
			~App();
			App(App const& window) = delete;
			App &operator=(App const) = delete;
			void run();
		private:
			void loadScopObjects();
	};

} // namespace scop

#endif
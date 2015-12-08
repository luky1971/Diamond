/*
    Copyright 2015 Ahnaf Siddiqui

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "D_SDLRenderer2D.h"

#include <iostream>
#include "SDL_image.h"
#include "Q_QuantumWorld2D.h"

#include "D_Launcher.h"
#include "D_SDLInput.h"
#include "D_SDLTexture.h"

namespace Diamond {
	namespace SDLRenderSpace {
		static inline void render(Diamond::SDLRenderObj2D &obj, SDL_Renderer *renderer) {
			Diamond::Transform2i transform = Quantum2D::QuantumWorld2D::transforms[obj.transform];
			SDL_Rect render_rect = {transform.position.x, transform.position.y, obj.size.x, obj.size.y};
			SDL_RenderCopyEx(renderer, obj.texture->texture, NULL, &render_rect, transform.rotation, NULL, obj.flip);
		}
	}
}


Diamond::SDLRenderer2D::SDLRenderer2D() : window(nullptr), renderer(nullptr) {}

bool Diamond::SDLRenderer2D::init() {
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		// TODO: Handle initialization failure and log
		std::cout << "SDL failed to initialize! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Create window
	window = SDL_CreateWindow(Launcher::config.game_name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
							  Launcher::config.window_width, Launcher::config.window_height, Launcher::config.window_width <= 0
							  || Launcher::config.window_height <= 0 ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
	if (window == nullptr) {
		// TODO: Handle window creation failure and log
		std::cout << "SDL failed to create window! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | (Launcher::config.vsync ? SDL_RENDERER_PRESENTVSYNC : 0x00000000));
	if (renderer == nullptr) {
		// TODO: Handle renderer creation failure and log
		std::cout << "SDL failed to create renderer! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}
	SDL_SetRenderDrawColor(renderer, Launcher::config.bg_color.r, Launcher::config.bg_color.g, Launcher::config.bg_color.b, Launcher::config.bg_color.a);

	// Initialize image loading
	int img_flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
	if (IMG_Init(img_flags) != img_flags) {
		// TODO: Handle image-loader initialization failure and log
		std::cout << "SDL_image failed to initialize! SDL_image Error: " << IMG_GetError() << std::endl;
		return false;
	}

	return true;
}

void Diamond::SDLRenderer2D::renderAll() {
	// Handle SDL events
	Input::resetKeyup();
	while (SDL_PollEvent(&e)) {
		SDLInput::handle(e);
	}

	// Render all the graphics
	//SDL_SetRenderDrawColor(renderer, Launcher::config.bg_color.r, Launcher::config.bg_color.g, Launcher::config.bg_color.b, Launcher::config.bg_color.a); // reset color
	SDL_RenderClear(renderer);
	for (std::vector<SDLRenderObj2D>::iterator i = render_objects.begin(); i != render_objects.end(); i++) {
		//std::cout << i->transform.position.x << " and " << i->transform.position.y << " and " << i->transform.scale << std::endl; // DEBUG
		SDLRenderSpace::render(*i, renderer);
	}

	// Update screen
	SDL_RenderPresent(renderer);
}

Diamond::Texture *Diamond::SDLRenderer2D::loadTexture(std::string path) {
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (surface == NULL) {
		// TODO: Handle image loading failure and log
		std::cout << "Failed to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
		return nullptr;
	}
	
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL) {
		// TODO: Handle texture creation failure and log
		std::cout << "Failed to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}
	int width = surface->w;
	int height = surface->h;
	
	SDL_FreeSurface(surface);
	return new SDLTexture(texture, width, height);
}

Diamond::RenderObj2D *Diamond::SDLRenderer2D::getRenderObj(renderobj_id render_obj) {
	return &render_objects[renderobj_id_index_map[render_obj]];
}

renderobj_id Diamond::SDLRenderer2D::genRenderObj(Texture *texture, transform2_id transform, float scale) {
	renderobj_id render_obj;
	if (renderobj_id_stack.size() > 0) {
		render_obj = renderobj_id_stack.back();
		renderobj_id_stack.pop_back();
		renderobj_id_index_map[render_obj] = render_objects.size();
	}
	else {
		render_obj = renderobj_id_index_map.size();
		renderobj_id_index_map.push_back(render_objects.size());
	}
	render_objects.push_back(SDLRenderObj2D(render_obj, texture, transform, scale));
	return render_obj;
}

void Diamond::SDLRenderer2D::freeRenderObj(renderobj_id render_obj) {
	tD_index index = renderobj_id_index_map[render_obj];
	if (index < render_objects.size() - 1) { // If in middle of vector, replace it with last element in vector
		render_objects[index] = render_objects.back();
		renderobj_id_index_map[render_objects[index].obj_id] = index;
	}
	render_objects.pop_back();
	renderobj_id_stack.push_back(render_obj);
}

Diamond::SDLRenderer2D::~SDLRenderer2D() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	
	IMG_Quit();
	SDL_Quit();
}

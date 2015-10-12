/*
 Copyright (c) 2015 Polymer Games
*/

#include "Graphics2D.h"

std::unique_ptr<Diamond::Renderer2D> Diamond::Graphics2D::renderer = nullptr;

void Diamond::Graphics2D::setRenderer(Renderer2D *renderer) {
	Graphics2D::renderer = std::unique_ptr<Renderer2D>(renderer);
}

bool Diamond::Graphics2D::init_renderer(Config &config) {
	return renderer->init(config);
}

void Diamond::Graphics2D::render() {
	renderer->render();
}

void Diamond::Graphics2D::gen_render_obj(GameObject2D *parent, Texture *texture, Transform2 &transform) {
	renderer->gen_render_obj(parent, texture, transform);
}
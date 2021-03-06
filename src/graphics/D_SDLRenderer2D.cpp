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

#include "D_Log.h"
#include "D_SDLRenderComponent2D.h"
#include "D_SDLTexture.h"
#include "D_Transform2.h"
#include "SDL_image.h"
#include <iostream>

Diamond::SDLRenderer2D::SDLRenderer2D(const Config &config, bool &success)
    : m_window(nullptr), m_renderer(nullptr), m_bgColor({0, 0, 0, 100}),
      m_renderCompPool(config.max_gameobjects_estimate) {
  success = true;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    Log::log("SDL failed to initialize! SDL Error: " +
             std::string(SDL_GetError()));
    success = false;
  }

  // Create window
  if (success) {
    auto flags = SDL_WINDOW_ALLOW_HIGHDPI;
    if (config.fullscreen || config.window_width <= 0 || config.window_height <= 0) {
      flags = (SDL_WindowFlags)(flags | SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    m_window = SDL_CreateWindow(
        config.game_name.c_str(), SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, config.window_width, config.window_height,
        flags);
    if (m_window == nullptr) {
      Log::log("SDL failed to create window! SDL Error: " +
               std::string(SDL_GetError()));
      success = false;
    }
  }

  // Create renderer
  if (success) {
    m_renderer = SDL_CreateRenderer(
        m_window, -1,
        SDL_RENDERER_ACCELERATED |
            (config.vsync ? SDL_RENDERER_PRESENTVSYNC : 0x00000000));
    if (m_renderer == nullptr) {
      Log::log("SDL failed to create renderer! SDL Error: " +
               std::string(SDL_GetError()));
      success = false;
    }
  }

  // Set background color
  m_bgColor = config.bg_color;

  // Initialize image loading
  int img_flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
  if (IMG_Init(img_flags) != img_flags) {
    Log::log("SDL_image failed to initialize! SDL_image Error: " +
             std::string(IMG_GetError()));
    // success = false; // TODO: what's wrong with SDL Image init on Android?
  }

  // Initialize text rendering
  if (TTF_Init() < 0) {
    Log::log("SDL_ttf failed to initialize! SDL_ttf Error: " +
             std::string(TTF_GetError()));
    success = false;
  }

  if (success && config.optimize_render_layers) {
    m_render_objects.m_render_objects.resize(config.num_render_layers_estimate);

    for (int i = 0; i < config.num_render_layers_estimate; ++i) {
      m_render_objects.m_render_objects[i].data().reserve(config.max_gameobjects_estimate);
    }
  }
}

Diamond::SDLRenderer2D::~SDLRenderer2D() {
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

void Diamond::SDLRenderer2D::renderAll() {
  // Render background
  SDL_SetRenderDrawColor(m_renderer, m_bgColor.r, m_bgColor.g, m_bgColor.b,
                         m_bgColor.a);
  SDL_RenderClear(m_renderer);

  // Render sprites
  for (auto l = m_render_objects.m_render_objects.begin(); l != m_render_objects.m_render_objects.end(); ++l) {
    for (auto i = l->begin(); i != l->end(); ++i) {
      const auto &transform = i->getTransform();

      auto pivot = i->pivot();
      auto &clip = i->clip();

      pivot.x *= transform.scale.x;
      pivot.y *= transform.scale.y;

      SDL_Rect render_rect = {
          (int)(transform.position.x - pivot.x), // render position x
          (int)(transform.position.y - pivot.y), // render position y
          (int)(clip.w * transform.scale.x),     // render width
          (int)(clip.h * transform.scale.y)      // render height
      };

      // Set transparency
      const auto &color = i->color();
      SDL_SetTextureColorMod(i->texture(), color.r, color.g, color.b);
      SDL_SetTextureAlphaMod(i->texture(), i->alpha());

      SDL_RenderCopyEx(
          m_renderer,         // The SDL backend renderer for this SDL instance.
          i->texture(),       // sprite
          &clip,              // source rect
          &render_rect,       // destination rect
          transform.rotation, // rotation for destination rect in degrees
          &pivot,             // rotation pivot location in local space
          i->getFlip()        // current flip status
          );

      // Reset the texture's transparency before the next render
      SDL_SetTextureColorMod(i->texture(), 255, 255, 255);
      SDL_SetTextureAlphaMod(i->texture(), 255);
    }
  }

  // Render points
  for (SDLRenderablePoint point : m_render_points_queue) {
    SDL_SetRenderDrawColor(m_renderer, point.color.r, point.color.g,
                           point.color.b, point.color.a);

    SDL_RenderDrawPoint(m_renderer, point.coords.x, point.coords.y);
  }

  m_render_points_queue.clear();

  // Render lines
  for (SDLRenderableLine line : m_render_lines_queue) {
    SDL_SetRenderDrawColor(m_renderer, line.color.r, line.color.g, line.color.b,
                           line.color.a);

    SDL_RenderDrawLine(m_renderer, line.p1.x, line.p1.y, line.p2.x, line.p2.y);
  }

  m_render_lines_queue.clear();

  // Update screen
  SDL_RenderPresent(m_renderer);
}

Diamond::Vector2<int> Diamond::SDLRenderer2D::getScreenResolution() const {
  SDL_DisplayMode mode;
  SDL_GetCurrentDisplayMode(0, &mode);
  return Vector2<int>(mode.w, mode.h);
}

Diamond::Vector2<int> Diamond::SDLRenderer2D::getResolution() const {
  Vector2<int> r;
  SDL_GL_GetDrawableSize(m_window, &(r.x), &(r.y));
  return r;
}

int Diamond::SDLRenderer2D::getRefreshRate() const {
  SDL_DisplayMode mode;
  SDL_GetCurrentDisplayMode(0, &mode);
  return mode.refresh_rate;
}

Diamond::DumbPtr<Diamond::Font>
Diamond::SDLRenderer2D::loadFont(const std::string &fontPath, int ptsize) {
  TTF_Font *font = TTF_OpenFont(fontPath.c_str(), ptsize);
  if (!font) {
    Log::log("Failed to load font " + fontPath +
             "! SDL_ttf Error: " + std::string(TTF_GetError()));
    return nullptr;
  }
  return DumbPtr<Font>(new SDLFont(font));
}

Diamond::DumbPtr<Diamond::Texture>
Diamond::SDLRenderer2D::loadTexture(std::string path) {
  SDL_Surface *surface = IMG_Load(path.c_str());
  if (surface == NULL) {
    Log::log("Failed to load image " + path +
             "! SDL_image Error: " + std::string(IMG_GetError()));
    return nullptr;
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, surface);
  if (texture == NULL) {
    Log::log("Failed to create texture from " + path +
             "! SDL Error: " + std::string(SDL_GetError()));
    return nullptr;
  }
  int width = surface->w;
  int height = surface->h;

  SDL_FreeSurface(surface);
  return DumbPtr<Texture>(new SDLTexture(texture, width, height));
}

Diamond::DumbPtr<Diamond::Texture>
Diamond::SDLRenderer2D::loadTextTexture(const std::string &text,
                                        const Font *font, const RGBA &color) {
  auto sdlfont = dynamic_cast<const SDLFont *>(font);
  if (sdlfont) {
    SDL_Color sdlcolor = {color.r, color.g, color.b, color.a};

    SDL_Surface *surface =
        TTF_RenderText_Solid(sdlfont->font, text.c_str(), sdlcolor);

    if (surface == NULL) {
      Log::log("Failed to load text " + text +
               ". SDL_ttf Error: " + std::string(TTF_GetError()));
      return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    if (texture == NULL) {
      Log::log("Failed to create texture for text " + text +
               ". SDL Error: " + std::string(SDL_GetError()));
      return nullptr;
    }
    int width = surface->w;
    int height = surface->h;

    SDL_FreeSurface(surface);
    // TODO: use memory pool!
    return DumbPtr<Texture>(new SDLTexture(texture, width, height));
  } else {
    Log::log("Given Font is not an SDLFont.");
  }
  return nullptr;
}

Diamond::DumbPtr<Diamond::RenderComponent2D>
Diamond::SDLRenderer2D::makeRenderComponent(const DTransform2 &transform,
                                            const Texture *texture,
                                            RenderLayer layer,
                                            const Vector2<tD_pos> &pivot) {
  // Make room for a new layer if necessary
  if ((int)layer > (int)m_render_objects.m_render_objects.size() - 1) {
    m_render_objects.m_render_objects.resize(layer + 1);
  }

  SDLrenderobj_id robj = m_render_objects.m_render_objects[layer].emplace(
      transform, dynamic_cast<const SDLTexture *>(texture), pivot);

  return m_renderCompPool.make(m_render_objects, robj, texture, layer);
}

void Diamond::SDLRenderer2D::renderPoint(const Vector2<tD_pos> &coords,
                                         const RGBA &color) {
  m_render_points_queue.push_back({coords, color});
}

void Diamond::SDLRenderer2D::renderLine(const Vector2<tD_pos> &p1,
                                        const Vector2<tD_pos> &p2,
                                        const RGBA &color) {
  m_render_lines_queue.push_back({p1, p2, color});
}



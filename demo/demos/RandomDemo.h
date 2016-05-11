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

#ifndef RANDOM_DEMO_H
#define RANDOM_DEMO_H

#include <list>
#include <memory>

#include "D_Animator2D.h"
#include "D_AnimatorSheet.h"
#include "D_Game2D.h"
#include "D_Entity2D.h"
#include "D_Rigidbody2D.h"
#include "D_typedefs.h"

class RandomDemo : public Diamond::Game2D {
public:
    RandomDemo(float movespeed = 0.75f, float spinspeed = 0.75f, float growspeed = 0.00075f);

    void init() override;

    void update(tD_delta delta) override;

    void quit() override;

private:
    const float movespeed, spinspeed, growspeed;

    Diamond::Entity2D *spike, *spike2, *zapper, *zapper2;

    std::shared_ptr<Diamond::Texture> spike_sprite;
    std::shared_ptr<Diamond::Texture> cloud_sprite;

    Diamond::Animation2D zapper_anim;
    Diamond::AnimationSheet zapper2_anim;

    Diamond::RGBA spike_color;
    std::unique_ptr<Diamond::Sound2D> haha;
    Diamond::Vector2<int> window;

    Diamond::Rigidbody2D *spikerb;
};

#endif // RANDOM_DEMO_H

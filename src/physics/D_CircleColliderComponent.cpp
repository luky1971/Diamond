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

#include "D_CircleColliderComponent.h"
#include "D_Entity2D.h"

Diamond::CircleColliderComponent::CircleColliderComponent(Entity2D *parent,
                                                          std::function<void(Entity2D *other)> &onCollision,
                                                          PhysicsWorld2D *phys_world,
                                                          tQ_pos radius,
                                                          const Vector2<tQ_pos> &center)
    : ColliderComponent2D(parent, onCollision, phys_world) {
    std::function<void(void*)> callback = std::bind(&CircleColliderComponent::onCollide, this, std::placeholders::_1);
    circle = phys_world->genCircleCollider(body, parent, callback, radius, center);
}

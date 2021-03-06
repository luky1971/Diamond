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

#ifndef Q_AABB_COLLIDER_2D_H
#define Q_AABB_COLLIDER_2D_H

#include "Q_Collider2D.h"
#include "duVector2.h"

namespace Quantum2D {
    class AABBCollider2D : public Collider2D {
    public:
        AABBCollider2D(const BodyList &bodylist, 
                       body2d_id body,
                       void *parent,
                       const std::function<void(void *other)> &onCollision,
                       const Diamond::Vector2<tQ_num> &dims, 
                       const Diamond::Vector2<tQ_num> &origin = Diamond::Vector2<tQ_num>(0, 0),
                       QLayer layer = 0);

        /**
         Get the origin point of this AABB in the parent rigidbody's local space.
        */
        const Diamond::Vector2<tQ_num> &getOrigin() const { return origin; }

        /**
         Get the horizontal x vertical dimensions of this AABB.
        */
        const Diamond::Vector2<tQ_num> &getDims() const { return dims; }

        /**
         Get the current coordinates of this AABB's bottom left corner in world space.
        */
        const Diamond::Vector2<tQ_num> &getMin() const { return min; }

        /**
         Get the current coordinates of this AABB's top right corner in world space.
        */
        const Diamond::Vector2<tQ_num> &getMax() const { return max; }

        void setOrigin(const Diamond::Vector2<tQ_num> &origin) { this->origin = origin; }

        void setDims(const Diamond::Vector2<tQ_num> &dims) { this->dims = dims; }

        /**
         Update world coordinates, physics world calls this once per frame.
        */
        void update(tQ_delta delta) override;

    private:
        Diamond::Vector2<tQ_num> origin;
        Diamond::Vector2<tQ_num> dims;

        Diamond::Vector2<tQ_num> min;
        Diamond::Vector2<tQ_num> max;
    };
}

#endif // Q_AABB_COLLIDER_2D_H

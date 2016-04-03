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

#ifndef D_QUANTUM_BODY_2D_H
#define D_QUANTUM_BODY_2D_H

#include "D_Rigidbody2D.h"
#include "Q_DynamicWorld2D.h"

namespace Diamond {
    class QuantumBody2D : public Rigidbody2D {
    public:
        QuantumBody2D(transform2_id transform, Quantum2D::DynamicWorld2D *world) : world(world) {
            body = world->genRigidbody(transform);
        }

        ~QuantumBody2D() {
            world->freeRigidbody(body);
        }
        
        body2d_id getID() const { return body; }

        Vector2<tD_pos> getVelocity() override {
            return world->getRigidbody(body).getVelocity();
        }

        void setVelocity(const Vector2<tD_pos> &newvel) override {
            world->getRigidbody(body).getVelocity() = newvel;
        }

    private:
        body2d_id body;
        Quantum2D::DynamicWorld2D *world;
    };
}

#endif // D_QUANTUM_BODY_2D_H

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

#ifndef D_MATH_H
#define D_MATH_H

#include "D_Vector2.h"

namespace Diamond {
    namespace Math {
        const float PI = 3.14159265;
        const float RAD2DEG = 180 / PI;
        const float DEG2RAD = PI / 180;

        inline float rad2deg(float rad) { return rad * RAD2DEG; }

        inline float deg2rad(float deg) { return deg * DEG2RAD; }

        template <typename A, typename B, typename C>
        inline bool leftOf(Vector2<C> &x, Vector2<A> &ea, Vector2<B> &eb) {
            return (eb.x - ea.x) * (x.y - ea.y) - (eb.y - ea.y) * (x.x - ea.x) > 0;
        }
    }
}

#endif // D_MATH_H

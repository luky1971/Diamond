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

#ifndef D_TRANSFORM_2_H
#define D_TRANSFORM_2_H

#include "D_Vector2.h"

namespace Diamond {
	class Transform2 {
		public:
		Transform2();
		Transform2(Vector2 position, float rotation, float scale);
		Vector2 position;
		float rotation;
		float scale;
	};
}

#endif // D_TRANSFORM_2_H

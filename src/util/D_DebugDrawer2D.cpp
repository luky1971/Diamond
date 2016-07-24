/*
    Copyright 2016 Ahnaf Siddiqui

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

#include "D_DebugDrawer2D.h"


Diamond::DebugDrawer::DebugDrawer(Renderer2D *renderer)
    : m_renderer(renderer) {}


void Diamond::DebugDrawer::draw(const Diamond::PolyCollider *poly,
                                const RGBA &color) {
    PointList2D points = poly->worldPoints();
    
    for (int i = 1; i < points.size(); ++i) {
        m_renderer->renderLine(points[i-1], points[i], color);
    }
    
    m_renderer->renderLine(points.back(), points.front(), color);
}

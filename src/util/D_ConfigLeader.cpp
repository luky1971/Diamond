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

#include "D_ConfigLoader.h"

bool Diamond::ConfigLoader::parseLine(const std::string &line,
                                      std::string &key,
                                      std::string &value) {
    auto i = line.find(':');

    if (i == std::string::npos ||
        i < 1 ||
        i + 1 == line.size())
        return false;

    key = line.substr(0, i);
    value = line.substr(i + 1);

    return true;
}
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

#include "D_Log.h"
#include "D_StdConfigLoader.h"
#include <fstream>

Diamond::ConfigTable Diamond::StdConfigLoader::load(const std::string &path) {
    ConfigTable config;

    std::ifstream fileStream;
    fileStream.open(path);

    if (fileStream.is_open()) {
        Log::log("Opened config file " + path + " for reading");

        std::string line, key, value;

        while (std::getline(fileStream, line)) {
            if (!line.empty()) {
                if (!parseLine(line, key, value)) {
                    Log::log("Failed to parse line " + line + " in " + path);
                }
                else {
                    config.set(key, value);
                }
            }
        }

        fileStream.close();
    }
    else {
        Log::log("Failed to open config file " + path + " for reading");
    }

    return config;
}


Diamond::StdConfigLoader::StdConfigLoader(const std::string &pathRoot)
    : m_pathRoot(pathRoot) {}

void Diamond::StdConfigLoader::write(const ConfigTable &table,
                                     const std::string &path) {
    std::ofstream filestream;
    filestream.open(path);

    if (filestream.is_open()) {
        Log::log("Opened config file " + path + " for writing");

        for (auto pair : table.data()) {
            // DEBUG
            // std::cout << pair.first << ":" << pair.second << std::endl;
            filestream << pair.first << ":" << pair.second << std::endl;
        }

        filestream.close();
    }
    else {
        Log::log("Failed to open config file " + path + " for writing");
    }
}

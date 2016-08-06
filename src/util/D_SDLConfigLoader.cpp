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

#include "D_SDLConfigLoader.h"

#include "SDL.h"
#include "D_Log.h"


Diamond::SDLConfigLoader::SDLConfigLoader(const std::string &pathRoot)
    : m_pathRoot(pathRoot) {}


Diamond::ConfigTable Diamond::SDLConfigLoader::load(const std::string &path) {
    ConfigTable config;
    std::string fullPath = m_pathRoot + path;

    SDL_RWops *file = SDL_RWFromFile(fullPath.c_str(), "r");

    if (file != NULL) {
        // Load file contents into buffer

        Sint64 size = SDL_RWsize(file);

        if (size <= 0) {
            Log::log("Could not determine size of config file, using "
                     "constant size buffer (" + toString(DEFAULT_BUFSIZE) +
                     " bytes)");
            size = DEFAULT_BUFSIZE;
        }

        char *buf = new char[size];
        char *bufptr = buf;

        Sint64 ntotal = 0, nread = 1;

        while (ntotal < size && nread != 0) {
            nread = SDL_RWread(file, bufptr, sizeof(char), size - ntotal);
            ntotal += nread;
            bufptr += nread;
        }

        // Done with the file
        SDL_RWclose(file);

        if (ntotal != size) {
            Log::log("WARNING: not all data from config file " + fullPath +
                     " was read (" + toString(ntotal) + " bytes read out of " +
                     toString(size) + ")");
        }

        // Parse each line in the buffer
        if (ntotal > 0) {
            std::string line, key, value;
            Status status;
            line.reserve(LINESIZE);

            for (int i = 0, begin = 0; i <= ntotal; ++i) {
                if (buf[i] == '\n' || (i == ntotal && i != begin)) {
                    line.assign(buf + begin, i - begin);

                    if (!configureLine(line, config)) {
                        Log::log("Failed to parse line " + line +
                                 " in " + fullPath);
                    }

                    begin = i + 1;
                }
            }
        }
        else {
            Log::log("Failed to read data from config file " + fullPath);
        }

        delete[] buf;
    }
    else {
        Log::log("Failed to open config file " + fullPath + " for reading");
    }

    return config;
}


bool Diamond::SDLConfigLoader::write(const ConfigTable &table,
                                     const std::string &path) {
    Log::log("Diamond does not support config file writing "
             "using SDL file ops.");

    return false;
}

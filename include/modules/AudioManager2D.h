/*
 Copyright (c) 2015 Polymer Games
*/

#ifndef D_AUDIO_MANAGER_2D_H
#define D_AUDIO_MANAGER_2D_H

#include "DiskJockey2D.h"

namespace Diamond {
	namespace AudioManager2D {

		/**
		 Loads a sound file as a sound effect. Currently only WAV files are supported.
		 Returns nullptr if sound failed to load.
		*/
		Sound2D *load_sound(std::string path);

		// Don't call unless you know what you're doing!
		void set_dj(DiskJockey2D *dj);
		bool init_dj(); // returns whether initialization was successful
	};
}

#endif // D_AUDIO_MANAGER_2D_H

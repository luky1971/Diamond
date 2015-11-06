/*
 Copyright (c) 2015 Polymer Games
*/

#include "D_Launcher.h"

#include <iostream>

#include "D_AudioManager2D.h"
#include "D_DesktopLogger.h"
#include "D_Graphics2D.h"
#include "D_Log.h"
#include "D_Time.h"

#if defined __ANDROID__ || defined IOS || _WIN32 || defined __APPLE__
#include "D_SDLDiskJockey2D.h"
#include "D_SDLRenderer2D.h"
#include "D_SDLTimer.h"
#endif

namespace Diamond {
	namespace Launcher {
		bool is_open = true;
		Config config = Config();
		
		static int nframes = 0;

		static void init_desktop() {
			Log::set_logger(new DesktopLogger());

			if (!Graphics2D::init_renderer(new SDLRenderer2D())) {
				// TODO: Handle renderer initialization failure
			}

			if (!AudioManager2D::init_dj(new SDLDiskJockey2D())) {
				// TODO: Handle audio initialization failure
			}
			
			Time::set_timer(new SDLTimer());
		}
	}
}

void Diamond::Launcher::launch(Game &game) {
#if defined __ANDROID__
	// Android launcher
#elif defined IOS // TODO: What is the IOS platform macro? Or define one manually!
	// IOS launcher
#elif defined _WIN32 || defined __APPLE__
	init_desktop(); // Desktop launcher (windows/osx)
#else
	// TODO: Log this using logger, and use a better RAII-compliant exit method! (ex. exceptions)
	std::cout << "Platform unsupported!" << std::endl;
	std::exit();
#endif

	uint32_t time;
	uint32_t last_time = Time::ms_elapsed();
	uint32_t delta;
	
	// Init game
	game.init();

	// Update
	while (Launcher::is_open) {
		nframes++;
		
		time = Time::ms_elapsed();
		delta = time - last_time;
		last_time = time;
		
		Time::fps = nframes / (time / 1000.0);
		
		game.update(delta);
		Graphics2D::render();
	}

	// End game
	game.quit();
}

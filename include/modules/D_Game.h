/*
 Copyright (c) 2015 Polymer Games
*/

#ifndef D_GAME_H
#define D_GAME_H

#include <stdint.h>

namespace Diamond {
	class Game {
		public:
		/**
		 Called at beginning of game, after Diamond backend systems have initialized.
		 Put game initialization code here.
		*/
		virtual void init() = 0;

		/**
		 Called once every frame, where delta is the time in seconds since the last frame.
		 Update game logic here.
		*/
		virtual void update(uint32_t delta) = 0;

		/**
		 Called when game ends.
		 Put cleanup code here.
		*/
		virtual void quit() = 0;
	};
}

#endif // D_GAME_H

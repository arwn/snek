#pragma once

#include "iview.hpp"
#include <curses.h>
#include <thread>
#include <atomic>
#include <unistd.h>

#define CTRLD 4

class curses_view: public iview {
	private:
		std::thread keythread;

		void keylisten();

	public:
		void init();
		void destroy();
		void flush_display();
		void draw_tile(BoardTile t, int y, int x);
		std::string prompt_user(std::string);
		void message_user(std::string);
};

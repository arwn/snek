#pragma once

#include "iview.hpp"
#include <curses.h>
#include <thread>
#include <atomic>
#include <unistd.h>

#define CTRLD 4

class curses2_view: public iview {
	private:
		WINDOW *scr;
		std::thread keythread;

		void keylisten();

	public:

		// generated code
		curses2_view(void);
		curses2_view(curses2_view const &);
		~curses2_view(void);
		curses2_view& operator=(curses2_view const &);
		// end generated code

		void init(int x, int y);
		void destroy();
		void flush_display();
		void draw_tile(BoardTile t, int y, int x);
		std::string prompt_user(std::string);
		void message_user(std::string);
		void sleep(void);
};

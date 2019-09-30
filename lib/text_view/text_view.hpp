#pragma once

#include "iview.hpp"
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::string;

class text_view: public iview {
	private:
		GameBoard board;

	public:
		void init(int x, int y);
		void destroy();
		void flush_display();
		void draw_tile(BoardTile t, int y, int x);
		std::string prompt_user(std::string);
		void message_user(std::string);
		void sleep(void);
};

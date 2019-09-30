#pragma once

#include <string>
#include "snake.hpp"

class iview {
	protected:
		int key = '?';

  	public:
		bool running = false;

		virtual void init(int x, int y) = 0;
		virtual void destroy() = 0;
		virtual void flush_display() = 0;
		virtual void draw_tile(BoardTile t, int y, int x) = 0;
		virtual std::string prompt_user(std::string msg) = 0;
		virtual void message_user(std::string msg) = 0;
		virtual void sleep(void) = 0;

		virtual ~iview() {};

		virtual int get_key(void) { return key; }
};

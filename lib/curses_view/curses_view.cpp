#include <iostream>
#include "curses_view.hpp"

// TODO remove key != CTRLD. quit game from menu.
// TODO add timeout to check if game is still running
void
curses_view::keylisten()
{
	while (running) {
		auto c = getchar();
		if (c != ERR)
			key = c;
	}
	running = false;
}

extern "C" iview*
make_view(void)
{
	return (new curses_view());
}

void
curses_view::init(int x, int y)
{
	// init ncurses
	scr = initscr();
	cbreak();
	noecho();
	nodelay(scr, true);
	keypad(scr, TRUE);

	running = true;

	// spawn listener thread
	keythread = std::thread(&curses_view::keylisten, this);
}

void
curses_view::destroy()
{
	endwin();
	// close(STDIN_FILENO);
	running = false;
	keythread.join();
}

void
curses_view::flush_display()
{
	wrefresh(scr);
}

void
curses_view::draw_tile(BoardTile t, int y, int x)
{
	mvaddch(y, x, t + 'a');
}


std::string
curses_view::prompt_user(std::string msg)
{
	waddstr(scr, "TODO: implement prompt user");
	wrefresh(scr);
	return nullptr;
}

void
curses_view::message_user(std::string msg)
{
	waddstr(scr, msg.c_str());
	wrefresh(scr);
}

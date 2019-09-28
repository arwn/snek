#include "curses_view.hpp"

void
curses_view::keylisten()
{
	while (key != CTRLD) {
		key = wgetch(stdscr);
	}
	running = false;
}

void
curses_view::init()
{
	// init ncurses
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	running = true;

	// spawn listener thread
	keythread = std::thread(&curses_view::keylisten, this);

}

void
curses_view::destroy()
{
	endwin();
	close(STDIN_FILENO);
	keythread.join();
}

void
curses_view::draw_screen(int **board, int size)
{
	waddstr(stdscr, "TODO: implement draw screen");
	wrefresh(stdscr);
}

std::string
curses_view::prompt_user(std::string msg)
{
	waddstr(stdscr, "TODO: implement prompt user");
	wrefresh(stdscr);
	return nullptr;
}

void
curses_view::message_user(std::string msg)
{
	waddstr(stdscr, msg.c_str());
	wrefresh(stdscr);
}


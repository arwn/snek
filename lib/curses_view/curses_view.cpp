#include <iostream>
#include "curses_view.hpp"

// generated code
curses_view::curses_view(void) { }
curses_view::curses_view(curses_view const &cp) { *this = cp; }
curses_view::~curses_view(void) { }
curses_view& curses_view::operator=(curses_view const &) { return *this; }	// TODO: implement
// end generated code

void
curses_view::keylisten()
{
	fd_set s_rd, s_wr, s_ex;
	static struct timeval time = {0,100};

	while (running) {
		FD_ZERO(&s_rd);
		FD_ZERO(&s_wr);
		FD_ZERO(&s_ex);
		FD_SET(fileno(stdin), &s_rd);
		if (!select(fileno(stdin)+1, &s_rd, &s_wr, &s_ex, &time))
			continue;
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
	(void)x;
	(void)y;
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
	(void)msg;
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

void
curses_view::sleep(void)
{
	::sleep(1);
}

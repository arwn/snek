#include <iostream>
#include "curses2_view.hpp"

// TODO remove key != CTRLD. quit game from menu.
// TODO add timeout to check if game is still running
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
	// init ncurses
	scr = initscr();
	cbreak();
	noecho();
	nodelay(scr, true);
	keypad(scr, TRUE);

	start_color();

	init_pair(1, COLOR_YELLOW, COLOR_MAGENTA);

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
	attron(COLOR_PAIR(1));
	mvaddch(y, x, 'z' - t);
	attroff(COLOR_PAIR(1));
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

void
curses_view::sleep(void)
{
	::sleep(1);
}

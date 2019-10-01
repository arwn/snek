#include <iostream>
#include "curses2_view.hpp"

// generated code
curses2_view::curses2_view(void) { }
curses2_view::curses2_view(curses2_view const &cp) { *this = cp; }
curses2_view::~curses2_view(void) { }
curses2_view& curses2_view::operator=(curses2_view const &) { return *this; }	// TODO: implement
// end generated code

void
curses2_view::keylisten()
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
	return (new curses2_view());
}

void
curses2_view::init(int x, int y)
{
	(void)x;
	(void)y;
	// init ncurses
	scr = initscr();
	cbreak();
	noecho();
	nodelay(scr, true);
	keypad(scr, TRUE);

	start_color();

	init_pair(1, COLOR_YELLOW, COLOR_MAGENTA);
	init_pair(2, COLOR_GREEN, COLOR_YELLOW);
	init_pair(3, COLOR_BLACK, COLOR_RED);
	init_pair(4, COLOR_GREEN, COLOR_WHITE);

	running = true;

	// spawn listener thread
	keythread = std::thread(&curses2_view::keylisten, this);
}

void
curses2_view::destroy()
{
	endwin();
	// close(STDIN_FILENO);
	running = false;
	keythread.join();
}

void
curses2_view::flush_display()
{
	wrefresh(scr);
}

void
curses2_view::draw_tile(BoardTile t, int y, int x)
{
	int pair = 1;
	if (t == Tail) {
		pair = 2;
	}
	else if (t == Head) {
		pair = 3;
	}
	else if (t == Fruit) {
		pair = 4;
	}
	attron(COLOR_PAIR(pair));
	mvaddch(y, x, 'z' - t);
	attroff(COLOR_PAIR(pair));
}

std::string
curses2_view::prompt_user(std::string msg)
{
	(void)msg;
	waddstr(scr, "TODO: implement prompt user");
	wrefresh(scr);
	return nullptr;
}

void
curses2_view::message_user(std::string msg)
{
	waddstr(scr, msg.c_str());
	wrefresh(scr);
}

void
curses2_view::sleep(void)
{
	::sleep(1);
}

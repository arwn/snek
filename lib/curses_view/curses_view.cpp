#include "curses_view.hpp"

// TODO remove key != CTRLD. quit game from menu.
// TODO add timeout to check if game is still running
void
curses_view::keylisten()
{
	while (key != CTRLD) {
		auto c = wgetch(stdscr);
		if (c != ERR)
			key = c;
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
	nodelay(stdscr, true);
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
curses_view::draw_screen(GameBoard board)
{
	auto row = 0;
	for (auto &y : board) {
		wmove(stdscr, row, 0);
		for (auto &x : y) {
			waddch(stdscr, x+'a');
		}
		row += 1;
	}
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


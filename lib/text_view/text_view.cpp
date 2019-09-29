#include "text_view.hpp"

extern "C" iview*
make_view(void)
{
	return (new text_view());
}

void
text_view::init()
{
	running = true;

	const static string clear_screen[] = {"\e[1;1H\e[2J"};
	cout << clear_screen;
}

void
text_view::destroy()
{
	init();
}

void
text_view::flush_display()
{
	fflush(stdout);
}

#include <curses.h>
void
text_view::draw_tile(BoardTile t, int y, int x)
{
	// intentionally left blank
	// flush_display prints the board
}

std::string
text_view::prompt_user(std::string msg)
{
	std::string done;
	cout << msg << endl;
	cin >> done;
	return done;
}

void
text_view::message_user(std::string msg)
{
	cout << msg << endl;
}

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

	// const static string clear_screen[] = {"\e[1;1H\e[2J"};
	// cout << clear_screen;
}

void
text_view::destroy()
{
	init();
}

void
text_view::flush_display()
{
	int dy = 0;
	int dx = 0;

	for (auto &row : board) {
		dx = 0;
		for (auto &col : row) {
			putchar(col + 'a');
			dx++;
		}
		putchar('\n');
		dy++;
	}
	putchar('\n');
	fflush(stdout);

	// maybe this should be moved
	key = getchar();
}

#include <curses.h>
void
text_view::draw_tile(BoardTile t, int y, int x)
{
	board[y][x] = t;
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

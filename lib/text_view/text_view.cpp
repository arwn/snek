#include "text_view.hpp"

extern "C" iview*
make_view(void)
{
	return (new text_view());
}

void
text_view::init(int x, int y)
{
	running = true;

	GameBoard b;
	for (int xx = 0; xx < x; xx++) {
		std::vector<BoardTile> row;
		for (int yy = 0; yy < y; yy++) {
			row.push_back(Empty);
		}
		b.push_back(row);
	}
	this->board = b;

	// const static string clear_screen[] = {"\e[1;1H\e[2J"};
	// cout << clear_screen;
}

void
text_view::destroy()
{
	// TODO: what the hell? why is this here?
	init(0, 0);
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

	std::cout << "Enter a direction (wasd)" << std::endl;
	char c;
	std::cin >> c;
	key = c;
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

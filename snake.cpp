#include <cstdio>
#include <unistd.h>
#include <vector>
#include <memory>
#include <iterator>

#include "curses_view/curses_view.hpp"
#include "snake.hpp"


static GameBoard board ={
	{Empty, Empty, Empty, Empty, Empty},
	{Empty, Empty, Empty, Empty, Empty},
	{Empty, Head, Empty, Empty, Empty},
	{Empty, Empty, Empty, Empty, Empty},
	{Fruit, Empty, Empty, Empty, Empty},
};

void
get_direction(int key, Direction *d)
{
	switch (key) {
		case 'w':
			*d = North; break;
		case 'a':
			*d = West; break;
		case 's':
			*d = South; break;
		case 'd':
			*d = East; break;
	}
}

// TODO: this function doesn't work, make it linked list.
void
move_snake(Direction d)
{
	for (auto y = 0; y < board.size(); y++) {
		for (auto x = 0; x < board.size(); x++) {
			if (board[y][x] == Head){
				switch (d) {
					case North:
						board[y-1][x] = Head; break;
					case South:
						board[y+1][x] = Head; break;
					case East:
						board[y][x+1] = Head; break;
					case West:
						board[y][x-1] = Head; break;
				}
			}
		}
	}
}

void
draw_board(std::shared_ptr<iview> view)
{
	int dy = 0;
	int dx = 0;

	for (auto &row : board) {
		dy++;
		dx = 0;
		for (auto &col : row) {
			dx++;
			view->draw_tile(col, dy, dx);
		}
	}
}

int
main(void)
{
	std::shared_ptr<iview> view = std::make_shared<curses_view>();
	view->init();
	Direction d;

	while (view->running == true) {
		get_direction(view->get_key(), &d);
		draw_board(view);
		view->flush_display();
		sleep(1);
	}

	view->destroy();
	return 0;
}

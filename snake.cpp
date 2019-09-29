#include <cstdio>
#include <dlfcn.h>
#include <iostream>
#include <tuple>
#include <list>
#include <unistd.h>
#include <vector>
#include <memory>
#include <iterator>

#include "curses_view/curses_view.hpp"
#include "snake.hpp"

static GameBoard board ={
	{Empty, Empty, Empty, Empty, Empty},
	{Empty, Empty, Empty, Empty, Empty},
	{Empty, Empty, Empty, Empty, Empty},
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
int
move_snake(Direction d, std::list<std::tuple<int, int>>& snek)
{
	auto head = snek.front();
	int x = std::get<0>(head);
	int y = std::get<1>(head);
	switch (d) {
	case North:
		--y;
		// board[y - 1][x] = Head;
		break;
	case South:
		++y;
		// board[y + 1][x] = Head;
		break;
	case East:
		++x;
		// board[y][x + 1] = Head;
		break;
	case West:
		--x;
		// board[y][x - 1] = Head;
		break;
	}
	if (x < 0 || y < 0)
		return 1;

	board[std::get<1>(head)][std::get<0>(head)] = Tail;

	auto back = snek.back();
	board[std::get<1>(back)][std::get<0>(back)] = Empty;
	snek.pop_back();

	snek.push_front(std::tuple<int, int>(x, y));
	board[y][x] = Head;
	// for (auto y = 0; y < board.size(); y++) {
	// 	for (auto x = 0; x < board.size(); x++) {
	// if (board[y][x] == Head) {
	// }
	// 	}
	// }
	return 0;
}

void
draw_board(iview* view)
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
	iview* view;
	void *lib = dlopen("./build/libtext_view.dylib", RTLD_NOW);
	if (!lib) {
		std::cout << "Lib no open 0 " << dlerror() << std::endl;
		return 1;
	}
	iview* (*fn)(void) = (iview*(*)())dlsym(lib, "make_view");
	if (!fn) {
		std::cout << "Function not opening 0 " << dlerror() << std::endl;
		return 1;
	}
	view = fn();

	// std::shared_ptr<iview> view = std::make_shared<curses_view>();
	view->init();
	Direction d = East;
	std::list<std::tuple<int, int>> snek;

	snek.push_front(std::tuple(3,3));
	while (view->running == true) {
		int key = view->get_key();
		get_direction(key, &d);
		int crash = move_snake(d, snek);
		draw_board(view);
		view->flush_display();
		sleep(1);
	}

	view->destroy();
	return 0;
}

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
		if (*d != South)
			*d = North;
		break;
	case 'a':
		if (*d != East)
			*d = West;
		break;
	case 's':
		if (*d != North)
			*d = South;
		break;
	case 'd':
		if (*d != West)
			*d = East;
		break;
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

	if (x < 0 || y < 0 || y >= board.size() || x >= board[y].size())
		return 1;

	board[std::get<1>(head)][std::get<0>(head)] = Tail;

	if (board[y][x] != Fruit) {
		auto back = snek.back();
		board[std::get<1>(back)][std::get<0>(back)] = Empty;
		snek.pop_back();
	}

	snek.push_front(std::tuple<int, int>(x, y));
	board[y][x] = Head;

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
load_lib(const char* libname, void** lib, iview** view)
{
	void* new_lib = NULL;
	iview* new_view = NULL;

	new_lib = dlopen(libname, RTLD_NOW);
	if (!new_lib) {
		std::cout << "Unable to open new_lib: " << dlerror() << std::endl;
		return 1;
	}
	iview* (*fn)(void) = (iview*(*)())dlsym(new_lib, "make_view");
	if (!fn) {
		std::cout << "Unable to open \"make_view\": " << dlerror() << std::endl;
		dlclose(new_lib);
		return 1;
	}
	new_view = fn();
	if (!new_view) {
		std::cerr << "Unable to create view" << std::endl;
		dlclose(new_lib);
		return 1;
	}

	*lib = new_lib;
	*view = new_view;
	return 0;
}

int
change_lib(const char* libname, void** lib, iview** view)
{
	iview* new_view = NULL;
	void* new_lib = NULL;

	if (load_lib(libname, &new_lib, &new_view)) {
		std::cerr << "Unable to change lib in change_lib" << std::endl;
		return 1;
	}
	if (*lib)
		dlclose(*lib);
	*lib = new_lib;
	*view = new_view;
	return 0;
}

const char*
libs[] = {
	"./build/libcurses_view.dylib",
};

int
main(void)
{
	iview* view = NULL;
	void* lib = NULL;

	if (change_lib(libs[0], &lib, &view)) {
		std::cerr << "Unable to change lib" << std::endl;
	}
	view->init();
	Direction d = East;
	std::list<std::tuple<int, int>> snek;

	snek.push_front(std::tuple(3, 3));

	while (view->running == true) {
		// view->destroy();
		// if (change_lib(libs[0], &lib, &view))
		// 	return 1;
		// view->init();

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

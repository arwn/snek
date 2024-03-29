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

static GameBoard board;

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

void
gen_fruit(void)
{
	std::vector<BoardTile*> empty;
	for (int yy = 0; yy < board.size(); ++yy) {
		for (int xx = 0; xx < board[yy].size(); ++xx) {
			if (board[yy][xx] == Empty)
				empty.push_back(&board[yy][xx]);
		}
	}
	if (empty.size() <= 0)
		return;
	*empty[rand() % empty.size()] = Fruit;
}

int
move_snake(Direction d, Snake& snek)
{
	auto head = snek.front();
	int x = std::get<0>(head);
	int y = std::get<1>(head);
	switch (d) {
	case North:
		--y;
		break;
	case South:
		++y;
		break;
	case East:
		++x;
		break;
	case West:
		--x;
		break;
	}

	if (x < 0 || y < 0 || y >= board.size() || x >= board[y].size()) {
		auto back = snek.back();
		board[std::get<1>(back)][std::get<0>(back)] = Empty;
		snek.pop_back();
		return snek.size() == 0;
	}

	board[std::get<1>(head)][std::get<0>(head)] = Tail;

	if (board[y][x] != Fruit) {
		auto back = snek.back();
		board[std::get<1>(back)][std::get<0>(back)] = Empty;
		snek.pop_back();
	} else {
		gen_fruit();
		board[y][x] = Empty;
	}

	if (board[y][x] != Empty) {
		board[std::get<1>(head)][std::get<0>(head)] = Head;
		return snek.size() == 0;
	}
	snek.push_front(std::tuple<int, int>(x, y));
	board[y][x] = Head;

	return snek.size() >= (board.size() * board[0].size());
}

void
draw_board(iview* view)
{
	int dy = 0;
	int dx = 0;

	for (auto &row : board) {
		dx = 0;
		for (auto &col : row) {
			view->draw_tile(col, dy, dx);
			dx++;
		}
		dy++;
	}
}

int
load_lib(const char* libname, void** lib, iview** view)
{
	void* new_lib = NULL;
	iview* new_view = NULL;

	new_lib = dlopen(libname, RTLD_NOW);
	if (!new_lib) {
		std::cerr << "Unable to open new_lib: " << dlerror() << std::endl;
		return 1;
	}
	iview* (*fn)(void) = (iview*(*)())dlsym(new_lib, "make_view");
	if (!fn) {
		std::cerr << "Unable to open \"make_view\": " << dlerror() << std::endl;
		dlclose(new_lib);
		return 1;
	}
	try {
		new_view = fn();
	}
	catch (std::exception& e) {
		std::cerr << "Unable to initialize view: " << e.what() << std::endl;
		return 1;
	}
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
	delete *view;
	if (*lib)
		dlclose(*lib);
	*lib = new_lib;
	*view = new_view;
	return 0;
}

GameBoard
make_board(int x, int y)
{
	GameBoard b;
	for (int yy = 0; yy < y; yy++) {
		std::vector<BoardTile> row;
		for (int xx = 0; xx < x; xx++) {
			row.push_back(Empty);
		}
		b.push_back(row);
	}
	return b;
}

const char*
libs[] = {
	"./build/libcurses_view.dylib",
	"./build/libtext_view.dylib",
	"./build/libcurses2_view.dylib",
};

int
main(int argc, char **argv)
{
	int x = 10;
	int y = 10;
	int libidx = 0;
	switch (argc) {
	case 4:
		libidx = atoi(argv[3]);
	case 3:
		y = atoi(argv[2]);
	case 2:
		x = atoi(argv[1]);
	case 1:
		break;
	default:
		std::cerr << "usage: " << argv[0] << " [x] [y]" << std::endl;
		return 1;
	}

	if (x < 5 || y < 5) {
		std::cerr << "Wait that's ridiculous" << std::endl;
		return 1;
	}

	srand(time(NULL));

	board = make_board(x, y);

	iview* view = NULL;
	void* lib = NULL;

	if (change_lib(libs[libidx % (sizeof(libs) / sizeof(*libs))], &lib, &view)) {
		std::cerr << "Unable to change lib" << std::endl;
		return 1;
	}
	Direction d = East;

	Snake snek;
	snek.push_front(std::tuple(x/2 - 2, y/2));
	board[y/2][x/2 - 2] = Tail;
	snek.push_front(std::tuple(x/2 - 1, y/2));
	board[y/2][x/2 - 1] = Tail;
	snek.push_front(std::tuple(x/2, y/2));
	board[y/2][x/2] = Head;

	gen_fruit();
	int crash = 0;

	try {
		view->init(x, y);
		while (view->running == true) {
			draw_board(view);
			view->flush_display();
			view->sleep();

			int key = view->get_key();
			if (key >= '0' && key <= '9') {
				view->destroy();
				change_lib(libs[(key - '0') % (sizeof(libs) / sizeof(*libs))],
						   &lib, &view);
				view->init(x, y);
				continue;
			}
			else if (key == 'q') {
				break;
			}
			get_direction(key, &d);
			crash = move_snake(d, snek);
			if (crash)
				break;
		}
	}
	catch (std::exception& e) {
		std::cerr << "Lib failed to execute: " << e.what() << std::endl;
	}
	try {
		view->destroy();
	}
	catch (std::exception& e) {
		std::cerr << "Unable to destroy: " << e.what() << std::endl;
	}
	delete view;
	dlclose(lib);

	std::cout << "Score: " << snek.size() << std::endl;
	if (crash)
		assert("Game over" && 0);
}

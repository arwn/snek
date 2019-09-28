#include <cstdio>
#include <unistd.h>
#include "curses_view/curses_view.hpp"

int main(void)
{
	iview *view = new curses_view();
	view->init();
	while (view->running == true) {
		view->draw_screen(nullptr, 0);
		sleep(1);
	}
	view->destroy();
	return 0;
}

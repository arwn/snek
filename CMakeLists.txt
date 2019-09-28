project (snake)
cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)

include_directories(lib .
	${CURSES_INCLUDE_DIR}
)

add_library(curses_view
	lib/curses_view/curses_view.cpp
	lib/curses_view/curses_view.hpp
)

add_executable(snake snake.cpp)


target_link_libraries(
	snake
	curses_view

	#libs
	${CURSES_LIBRARIES}
)

project (snake)
cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

find_package(Curses REQUIRED)

include_directories(
	.
	lib
	${CURSES_INCLUDE_DIR}
)

add_library(curses_view SHARED
	lib/curses_view/curses_view.cpp
	lib/curses_view/curses_view.hpp
)

add_library(text_view SHARED
	lib/text_view/text_view.cpp
	lib/text_view/text_view.hpp
)

add_executable(snake snake.cpp)

target_link_libraries(
	curses_view
	text_view

	#external
	${CURSES_LIBRARIES}
)

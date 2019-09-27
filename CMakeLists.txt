cmake_minimum_required(VERSION 3.15)

project(Snake)

add_library(hello
	lib/snake/snake.cpp
	lib/hello/hello.hpp
	lib/hello/hello.cpp
)

add_executable(snake
	lib/snake/snake.cpp
)

target_link_libraries(snake hello)

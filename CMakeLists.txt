cmake_minimum_required(VERSION 2.8.9)

project (snake)

include_directories(lib)

add_library(hello_snake
	lib/hello_snake/hello.cpp
	lib/hello_snake/hello.hpp
)

add_executable(snake hello.cpp)

target_link_libraries(snake hello_snake)

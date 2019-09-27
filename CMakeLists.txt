project (snake)
cmake_minimum_required(VERSION 2.8.9)

include_directories(lib)

add_library(hello_snake
	lib/hello_snake/hello.cpp
	lib/hello_snake/hello.hpp
)

add_executable(snake snake.cpp)

target_link_libraries(snake hello_snake)

set_property(TARGET snake PROPERTY CXX_STANDARD 17)

#pragma once

#include <list>
#include <tuple>
#include <vector>

enum BoardTile {
	Empty,
	Head,
	Tail,
	Fruit,
};

enum Direction {
	North,
	South,
	East,
	West,
};

using GameBoard = std::vector<std::vector<BoardTile>>;
using Snake = std::list<std::tuple<int, int>>;

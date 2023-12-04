#include "Centers.hpp"
#include "Cube.hpp"
#include <iostream>

namespace solver {

void Centers::solveCenters(Cube &cube) {
	if (cube.cubies[4][Cube::up] != Cube::up) {
		if (cube.cubies[4][Cube::front] == Cube::up) {
			cube.moves("MMM");
		} else if (cube.cubies[4][Cube::down] == Cube::up) {
			cube.moves("MM");
		} else if (cube.cubies[4][Cube::back] == Cube::up) {
			cube.moves("M");
		} else if (cube.cubies[4][Cube::left] == Cube::up) {
			cube.moves("S");
		} else if (cube.cubies[4][Cube::right] == Cube::up) {
			cube.moves("SSS");
		}
	}
	while (cube.cubies[4][Cube::front] != Cube::front) {
		cube.moves("E");
	}		
}

} // namespace solver
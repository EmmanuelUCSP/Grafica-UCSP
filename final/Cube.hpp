#ifndef CUBE_HPP
#define CUBE_HPP

#include <iostream>
#include <array>

namespace solver {

/*
right	green   3
left	blue  	5
up		yellow  1
down	white   0
front	red   	2
back	orange  4
*/

class Cube {
public:
	// using CubeArray = std::array<std::array<int,9>,6>;
	int cubies[9][6];
	std::string solution;
	
	enum Side {
		right = 3,
		left = 5,
		up = 1,
		down = 0,
		front = 2,
		back = 4,
	};

  /* Set up Cube.
  If scramble set, call scramble.
  Else, set up a solved cube */
  Cube(bool);

  /* By default scramble sets
  cubies to a random state,
  using combination of move functions */
  void scramble();

  std::string printSequence(std::string);
  void moves(std::string, bool=true);
  void output();
  void R(int);
  void L(int);
  void U(int);
  void D(int);
  void F(int);
  void B(int);
  void M(int);
  void E(int);
  void S(int);
};

} // namespace solver

#endif // CUBE_HPP

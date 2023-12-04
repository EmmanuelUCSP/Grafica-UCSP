#ifndef CROSS_H
#define CROSS_H

#include "Cube.hpp"

namespace solver {

class Cross {
public:
  static void solveCross(Cube&);
  static int checkCross(Cube&);
  static void solveEdge(Cube&);
};

} // namespace solver

#endif

#ifndef EDGES_H
#define EDGES_H

#include "Cube.hpp"

namespace solver {

class Edges {

public:
  static void solveEdges(Cube &cube);
private:
  static void solveEdge(Cube &cube);
  static int checkEdges(Cube &cube);
};

} // namespace solver

#endif

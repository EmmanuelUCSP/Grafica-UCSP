#ifndef CORNERS_H
#define CORNERS_H

#include "Cube.hpp"
#include "Cross.hpp"

namespace solver {

class Corners {

public:
  static void solveCorners(Cube&);
private:
  static int checkCorners(Cube&);
  static int checkBottomLayer(Cube &cube);

};

} // namespace solver

#endif

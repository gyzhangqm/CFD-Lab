#ifndef _LBDEFINITIONS_H_
#define _LBDEFINITIONS_H_

  static const int LATTICEVELOCITIES[19][3] = {{0, -1.0, -1.0}, {-1.0, 0, -1.0}, {0, 0, -1.0}, {1.0, 0, -1.0}, {0, 1.0, -1.0}, {-1.0, -1.0, 0}, {0, -1.0, 0},
						{1.0, -1.0, 0}, {-1.0, 0, 0}, {0, 0, 0}, {1.0, 0, 0}, {-1.0, 1.0, 0}, {0, 1.0, 0}, {1.0, 1.0, 0},
						{0, -1.0, 1.0}, {-1.0, 0, 1.0}, {0, 0, 1.0}, {1.0, 0, 1.0}, {0, 1.0, 1.0}};

  static const double LATTICEWEIGHTS[19] = {1.0/36.0, 1.0/36.0, 1.0/18.0, 1.0/36.0, 1.0/36.0, 1.0/36.0, 1.0/18.0, 1.0/36.0, 1.0/18.0, 1.0/3.0,
						1.0/18.0, 1.0/36.0, 1.0/18.0, 1.0/36.0, 1.0/36.0, 1.0/36.0, 1.0/18.0, 1.0/36.0, 1.0/36.0};
  static const double C_S = 0.577350269189626;


  static const int FLUID = 0;
  static const int NO_SLIP = 1;
  static const int MOVING_WALL = 2;

  static const int D = 3;
  static const int Q = 19;

#endif


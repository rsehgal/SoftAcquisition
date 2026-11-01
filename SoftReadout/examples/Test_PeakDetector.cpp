/*
**	Filename : Test_PeakDetector.cpp
**	2026-11-01
**	username : rsehgal
*/
#include "PeakDetector.h"
#include <iostream>
int main(int argc, char *argv[]) {
  PeakDetector pd;
  pd.Print();

  PeakDetector::Config cfg;
  cfg.activationNs = 4;
  cfg.holdDelayNs = 108;

  PeakDetector pd2(cfg);
  pd2.Print();

  return 0;
}

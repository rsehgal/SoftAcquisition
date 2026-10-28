/*
**	Filename : Test_Preamp.cpp
**	2026-10-27
**	username : rsehgal
*/
#include <iostream>
#include "Preamp.h"
#include "globals.h"
#include "SiPM.h"
int main(int argc, char *argv[])
{
  Preamp::Config cfg;
  cfg.gType   = Preamp::LOW_GAIN;
  cfg.fCIndex = 61;

  Preamp pa(cfg);
  pa.Print();

  std::cout << pa.Process(1.009) << std::endl;

  SiPM u(1, 22292, 6, 6);
  double xc = 1.3;
  double yc = -1.3;
  std::cout << "Microcell Id of coordinate (1.3,-1.3) : " << u.MicrocellId(xc, yc) << std::endl;
  u.GetMicrocell(xc, yc).Print();
  return 0;
}

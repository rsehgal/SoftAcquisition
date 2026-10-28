/*
**	Filename : Test_Preamp.cpp
**	2026-10-27
**	username : rsehgal
*/
#include <iostream>
#include "Preamp.h"
#include "globals.h"
#include "SiPM.h"
#include "TGraph.h"
#include "TApplication.h"

int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("fApp",NULL,NULL);

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

  std::cout << "========= Generating Waveform from Preamp =========" << std::endl;
  Avalanche av(987.,35);
  std::vector<double> waveform = pa.Waveform(av);
  std::vector<double> timeVec = pa.GetTimeVec();

  TGraph *gr=new TGraph(timeVec.size(),&timeVec[0],&waveform[0]);
  gr->Draw();

  fApp->Run();
  return 0;
}

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
#include "DataReader.h"

int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("fApp", NULL, NULL);

  Preamp::Config cfg;
  cfg.gType   = Preamp::HIGH_GAIN;
  cfg.fCIndex = 50;

  Preamp pa(cfg);
  pa.Print();

  SiPM u(1, 22292, 6, 6);
  u.Reset();
  Photon ph1(1.5, 1.5, 45.);
  Photon ph2(0.5, -1.3, 25.);
  std::vector<Photon> photonsVec = {ph1}; //, ph2};

  std::vector<std::vector<double>> vecOfWaveform;

  std::vector<Avalanche> vecOfAvalanche = u.GetAvalancheVec(photonsVec, ChargeMode::CrosstalkAndAfterpulse);

  for (const auto &av : vecOfAvalanche) {
    vecOfWaveform.push_back(pa.Waveform(av));
  }

  std::cout << "========= Size of Avalanche Vector : " << vecOfAvalanche.size() << " ========" << std::endl;
  std::cout << "========= Generating Resultant Waveform from Preamp =========" << std::endl;
  std::vector<double> resWaveform = pa.Waveform(vecOfAvalanche);

  std::vector<double> timeVec = pa.GetTimeVec();

  TGraph *gr = new TGraph(timeVec.size(), &timeVec[0], &resWaveform[0]);
  gr->Draw();

  fApp->Run();
  return 0;
}

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

  std::vector<double> timeVec = pa.GetTimeVec();

  DataReader *r                  = new DataReader(argv[1], "ftree");
  std::vector<Photon> photonsVec = r->GetPhotonsVec(10);
  sort(photonsVec.begin(), photonsVec.end(), compareByArrivalTime);
  std::cout << "Size of Photon Vector : " << photonsVec.size() << std::endl;

  //Forcefull setting the gain to 1 for cross-checking the plot and calculated charge values
  pa.SetGainFactor(1.);
  pa.Print();


  SiPM v(1, 22292, 6, 6);
  unsigned int seed = std::random_device{}();
  v.SetSeed(seed);
  v.Reset();

  std::vector<Avalanche> vecOfAvalanche = v.GetAvalancheVec(photonsVec, ChargeMode::CrosstalkOnly);
  double ic = v.GetIntegratedCharge(vecOfAvalanche);

  std::cout << "========= Size of Avalanche Vector : " << vecOfAvalanche.size() << " ========" << std::endl;
  std::cout << "====  Total Integrated Charge : " << ic << " ======" << std::endl;
  std::cout << "========= Generating Resultant Waveform from Preamp =========" << std::endl;
   std::vector<double> resWaveform = pa.Waveform(vecOfAvalanche);
  TGraph *gr  = new TGraph(timeVec.size(), &timeVec[0], &resWaveform[0]);
  gr->Draw();

  fApp->Run();
  return 0;
}

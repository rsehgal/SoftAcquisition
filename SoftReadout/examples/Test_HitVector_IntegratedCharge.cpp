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

  //std::vector<double> resWaveform = pa.Waveform(vecOfAvalanche);
  std::vector<double> timeVec = pa.GetTimeVec();

  //TGraph *gr = new TGraph(timeVec.size(), &timeVec[0], &resWaveform[0]);
  //gr->Draw();

  DataReader *r                                = new DataReader(argv[1], "ftree");
  std::vector<Photon> photonVec                = r->GetPhotonsVec(10);
  sort(photonVec.begin(), photonVec.end(), compareByArrivalTime); 
  double icWithoutCrosstalk                    = 0.;
  double icWithCrosstalk                       = 0.;
  double icWithCrosstalkAndAfterpulse          = 0.;
  double icWithCrosstalkAfterpulseAndDarkCount = 0.;

  SiPM v(1, 22292, 6, 6);
  //Increase n to reduce the statistical fluctuation
  unsigned int n = 1;
  for (unsigned int i = 0; i < n; i++) {
    // std::cout << "--------------------------------------" << std::endl;
    unsigned int seed = std::random_device{}();
    // std::cout << "seed = " << seed << std::endl;
    v.SetSeed(seed);

    v.Reset();
    double ic = v.GetIntegratedCharge(v.GetHitsVec(photonVec, ChargeMode::NoCrosstalk));
    icWithoutCrosstalk += ic;
    // std::cout << "Integrated Charge Without Crosstalk : " << ic << std::endl;

    v.Reset();
    ic = v.GetIntegratedCharge(v.GetHitsVec(photonVec, ChargeMode::CrosstalkOnly));
    icWithCrosstalk += ic;
    // std::cout << "Integrated Charge With Crosstalk : " << ic << std::endl;

    v.Reset();
    ic = v.GetIntegratedCharge(v.GetHitsVec(photonVec, ChargeMode::CrosstalkAndAfterpulse));
    icWithCrosstalkAndAfterpulse += ic;

    v.Reset();
    ic = v.GetIntegratedCharge(v.GetHitsVec(photonVec, ChargeMode::CrosstalkAfterpulseDarkCount));
    icWithCrosstalkAfterpulseAndDarkCount += ic;

    // std::cout << "Integrated Charge With Crosstalk and Afterpulses : "
    //         << ic << std::endl;
  }
  icWithoutCrosstalk /= n;
  icWithCrosstalk /= n;
  icWithCrosstalkAndAfterpulse /= n;
  icWithCrosstalkAfterpulseAndDarkCount /= n;

  std::cout << "Average Integrated Charge Without Crosstalk : " << icWithoutCrosstalk << std::endl;
  std::cout << "Average Integrated Charge With Crosstalk : " << icWithCrosstalk << std::endl;
  std::cout << "Average Integrated Charge With Crosstalk and Afterpulse: " << icWithCrosstalkAndAfterpulse << std::endl;
  std::cout << "Average Integrated Charge With Crosstalk Afterpulse and DarkCurrent : "
            << icWithCrosstalkAfterpulseAndDarkCount << std::endl;

  fApp->Run();
  return 0;
}

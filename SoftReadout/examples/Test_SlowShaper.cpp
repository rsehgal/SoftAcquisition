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
#include "SlowShaper.h"
#include "TCanvas.h"
#include "ReadoutHelpers.h"
int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("fApp", NULL, NULL);

  Preamp::Config cfg;
  cfg.gType   = Preamp::LOW_GAIN;
  cfg.fCIndex = 61;

  Preamp pa(cfg);
  pa.Print();

  std::cout << "========= Generating Waveform from Preamp =========" << std::endl;
  Avalanche av(987., 35);
  std::vector<double> waveform = pa.Waveform(av);
  std::vector<double> timeVec  = pa.GetTimeVec();

  SlowShaper sh;
  sh.Print();
  std::vector<double> out = sh.Process(waveform, timeVec);

  new TCanvas("PreAmp", "PreAmp");
  TGraph *gr = new TGraph(timeVec.size(), &timeVec[0], &waveform[0]);
  gr->SetLineColor(4);
  gr->Draw();

  TGraph *grSh = new TGraph(timeVec.size(), &timeVec[0], &out[0]);
  grSh->SetLineColor(2);
  grSh->Draw("same");


  double tp;
  double pk = MaxOf(out, timeVec, tp);
  std::printf("\n---- single avalanche ----\npreamp plateau %.2f\nshaper peak %.2f at t = %.1f ns (%.1f ns after the avalanche)\n"
              "ratio peak/plateau = %.4f   (expected 0.658 for 50 ns, n = 2)\n",
              waveform.back(), pk, tp, tp - av.arrivalTime, pk / waveform.back());
  /*
  std::vector<TGraph *> vecOfGr;
  for (unsigned int i = 1; i < 5; i++) {
    sh.SetOrder(i);
    std::vector<double> out = sh.Process(waveform, timeVec);
    vecOfGr.push_back(new TGraph(timeVec.size(), &timeVec[0], &out[0]));
  }

  for (unsigned int i = 0; i < vecOfGr.size(); i++) {
    vecOfGr[i]->SetLineColor(i);
    vecOfGr[i]->Draw("same");
  }
  */
  fApp->Run();
  return 0;
}

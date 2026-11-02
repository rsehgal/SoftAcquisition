/*
**	Filename : Test_PeakDetector.cpp
**	2026-11-01
**	username : rsehgal
*/
#include "PeakDetector.h"
#include <iostream>
#include "Preamp.h"
#include "SlowShaper.h"
#include "TApplication.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TLine.h"
#include "TMultiGraph.h"
int main(int argc, char *argv[])
{

  TApplication *fApp = new TApplication("fApp", NULL, NULL);

  TMultiGraph *mg = new TMultiGraph();
  mg->SetTitle("Preamp,Shaper and peak detector output;time (ns);amplitude (units of q)");

  Preamp::Config cfgPre;
  Preamp pa(cfgPre);
  pa.SetGainFactor(1.);
  pa.SetTimeAxis(0, 2500);
  std::vector<double> timeVec = pa.GetTimeVec();
  std::cout << "Size of timeVec : " << timeVec.size() << std::endl;

  pa.SetDecayNs(300.);
  pa.Print();
  Avalanche av(1., 0.);
  std::vector<double> pre = pa.Waveform(av);
  TGraph *grPre           = new TGraph(timeVec.size(), &timeVec[0], &pre[0]);
  grPre->SetLineWidth(2);
  mg->Add(grPre,"L");

  //SlowShaper sh;
  //sh.SetShapingIndex(4);
  SlowShaper sh(SlowShaper::Config{4, 0});

  sh.Print();
  std::vector<double> shaped = sh.Process(pre, timeVec);
  TGraph *grSh               = new TGraph(timeVec.size(), &timeVec[0], &shaped[0]);
  grSh->SetLineWidth(2);
  grSh->SetLineColor(2);
  mg->Add(grSh,"L");

  //grPre->Draw();
  //grSh->Draw("same");

  mg->Draw("A"); 

  PeakDetector::Config cfgPd;
  cfgPd.activationNs = 4;
  cfgPd.holdDelayNs  = 108;

  std::vector<bool> trgVec(timeVec.size(), false);
  trgVec[1050] = true;
  PeakDetector pd(cfgPd);
  pd.Print();
  std::vector<PeakDetector::Result> resVec = pd.Process(timeVec, shaped, trgVec);

  std::cout << "======================================" << std::endl;
  for (const auto &res : resVec) {
    TLine *ln = new TLine(0.,res.vHold ,timeVec[timeVec.size()-1],res.vHold);
    ln->SetLineStyle(2);
    ln->SetLineWidth(2);
    ln->Draw("same");
    res.Print();
  }
  gPad->SetGrid();

 fApp->Run();

  return 0;
}

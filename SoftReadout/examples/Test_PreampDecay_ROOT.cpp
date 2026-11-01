/*
**	Filename : Test_PreampDecay_ROOT.cpp
**	One avalanche -> Preamp (with and without decay) -> SlowShaper.
**	Everything is drawn on ONE canvas: left pad = preamp output, right pad = slow shaper output.
**	Usage : ./Test_PreampDecay_ROOT [shapingIndex(1..7, default 4 = 50 ns)]
*/
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "Preamp.h"
#include "SlowShaper.h"
#include "globals.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include "TAxis.h"

int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("fApp", NULL, NULL);
  unsigned short shapingIdx = (argc > 1) ? std::atoi(argv[1]) : 4;

  Preamp::Config cfg;
  Preamp pa(cfg);
  pa.SetGainFactor(1.);       // output in units of the SiPM charge: plateau = 1 for q = 1
  pa.SetTimeAxis(0, 2500);    // long enough to see the recovery of the slow decays
  std::vector<double> timeVec = pa.GetTimeVec();

  SlowShaper sh(SlowShaper::Config{shapingIdx, 0});
  sh.Print();

  Avalanche av(1., 0.);       // unit charge arriving at t = 0 ns

  // decay times in ns (0 = no decay)
  std::vector<double> decays = {0., 1000., 300., 100.};
  std::vector<int> colors    = {kBlack, kGreen + 2, kOrange + 1, kRed};

  TMultiGraph *mgPre = new TMultiGraph();
  TMultiGraph *mgSh  = new TMultiGraph();
  mgPre->SetTitle("Preamp output;time (ns);amplitude (units of q)");
  mgSh->SetTitle("Slow shaper output;time (ns);amplitude (units of q)");
  TLegend *leg = new TLegend(0.55, 0.60, 0.88, 0.88);
  leg->SetBorderSize(0);

  std::cout << "\ndecay(ns)  shaped peak  t_peak(ns)  undershoot  t_min(ns)" << std::endl;
  for (unsigned int k = 0; k < decays.size(); k++) {
    pa.SetDecayNs(decays[k]);
    std::vector<double> pre    = pa.Waveform(av);
    std::vector<double> shaped = sh.Process(pre, timeVec);

    size_t ip = std::max_element(shaped.begin(), shaped.end()) - shaped.begin();
    size_t im = std::min_element(shaped.begin(), shaped.end()) - shaped.begin();
    std::cout << decays[k] << "\t" << shaped[ip] << "\t" << timeVec[ip] << "\t" << shaped[im] << "\t" << timeVec[im] << std::endl;

    TGraph *grPre = new TGraph(timeVec.size(), &timeVec[0], &pre[0]);
    TGraph *grSh  = new TGraph(timeVec.size(), &timeVec[0], &shaped[0]);
    for (TGraph *g : {grPre, grSh}) {
      g->SetLineColor(colors[k]);
      g->SetLineWidth(2);
    }
    mgPre->Add(grPre, "L");
    mgSh->Add(grSh, "L");
    std::string label = (decays[k] <= 0.) ? "no decay" : "decay " + std::to_string((int)decays[k]) + " ns";
    leg->AddEntry(grSh, label.c_str(), "l");
  }

  TCanvas *c = new TCanvas("cDecay", "Preamp decay", 1300, 520);
  c->Divide(2, 1);

  c->cd(1);
  mgPre->Draw("A");
  mgPre->GetXaxis()->SetRangeUser(-100., 2500.);
  leg->Draw();
  gPad->SetGrid();

  c->cd(2);
  mgSh->Draw("A");
  mgSh->GetXaxis()->SetRangeUser(-100., 1500.);
  leg->Draw();
  gPad->SetGrid();

  c->Update();
  fApp->Run();
  return 0;
}

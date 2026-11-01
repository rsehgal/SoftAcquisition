/*
**	Filename : Test_PreampDecay.cpp
**	No ROOT needed. Preamp with a decay time -> slow shaper. Writes decay_scan.csv for plotting.
**	Checks: (1) decay = 0 reproduces the old plateau, (2) exact formula vs a step-by-step leaky integrator,
**	(3) peak / undershoot table for several decays, (4) area of the shaped pulse -> 0, (5) superposition,
**	(6) tauD == tauF special case.
*/
#include "Preamp.h"
#include "SlowShaper.h"
#include "globals.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>

static void PeakMin(const std::vector<double> &v, const std::vector<double> &t, double &pk, double &tpk, double &mn, double &tmn)
{
  size_t ip = std::max_element(v.begin(), v.end()) - v.begin();
  size_t im = std::min_element(v.begin(), v.end()) - v.begin();
  pk = v[ip]; tpk = t[ip]; mn = v[im]; tmn = t[im];
}

int main()
{
  Preamp::Config cfg;
  Preamp pa(cfg);
  pa.SetGainFactor(1.);          // work in units of the SiPM charge: output = 1 for q = 1 on the plateau
  pa.SetTimeAxis(0, 3000);       // from -500 ns to 3000 + 250 + 525 ns
  const std::vector<double> t = pa.GetTimeVec();
  const double dt = t[1] - t[0];
  const Avalanche av(1., 0.);    // unit charge at t = 0

  SlowShaper sh;                 // 50 ns, CR-RC^2
  sh.Print();

  // (1) decay = 0 must be the old formula
  pa.SetDecayNs(0.);
  std::vector<double> x0 = pa.Waveform(av);
  double maxdiff = 0.;
  for (size_t i = 0; i < t.size(); i++) {
    double ref = (t[i] > 0) ? 1. - std::exp(-t[i] / kTauRecharge) : 0.;
    maxdiff = std::max(maxdiff, std::fabs(x0[i] - ref));
  }
  std::printf("\n(1) decay = 0 vs old formula: max difference = %.2e (expect 0)\n", maxdiff);

  // (2) exact formula vs numerical leaky integrator  dx/dt = i - x/tauD  (trapezoid steps)
  {
    const double tauD = 300.;
    pa.SetDecayNs(tauD);
    std::vector<double> x = pa.Waveform(av);
    double v = 0., iPrev = 0., md = 0., mdLate = 0.;
    for (size_t k = 0; k < t.size(); k++) {
      double i = (t[k] > 0) ? std::exp(-t[k] / kTauRecharge) / kTauRecharge : 0.;
      double c = 2. * tauD / dt;                     // bilinear: v' = i - v/tauD
      v = ((i + iPrev) * tauD + (c - 1.) * v) / (c + 1.);
      iPrev = i;
      md = std::max(md, std::fabs(v - x[k]));
      if (t[k] > 100.) mdLate = std::max(mdLate, std::fabs(v - x[k]));
    }
    std::printf("(2) exact preamp formula vs step-by-step integrator (tauD = 300): max difference = %.2e overall, %.2e for t > 100 ns\n"
                "    (the overall value equals dt/2 * i(0) = %.1e: start-up error of the numerical integrator, not of the formula)\n",
                md, mdLate, 0.5 * dt / kTauRecharge);
  }

  // (3) scan
  std::printf("\n(3) slow shaper 50 ns, n = 2, one avalanche (q = 1)\n");
  std::printf("%-10s %-10s %-9s %-11s %-10s %-9s %-9s\n", "decay(ns)", "preamp pk", "shp peak", "t_peak(ns)", "undershoot", "t_min(ns)", "area");
  std::ofstream csv("decay_scan.csv");
  csv << "t";
  const double decays[] = {0., 2000., 1000., 300., 100.};
  std::vector<std::vector<double>> shapedAll;
  for (double d : decays) csv << ",pre_" << d << ",shaped_" << d;
  csv << "\n";
  std::vector<std::vector<double>> preAll;
  for (double d : decays) {
    pa.SetDecayNs(d);
    std::vector<double> x = pa.Waveform(av);
    std::vector<double> y = sh.Process(x, t);
    double pk, tpk, mn, tmn, ppk, tppk, pmn, tpmn;
    PeakMin(y, t, pk, tpk, mn, tmn);
    PeakMin(x, t, ppk, tppk, pmn, tpmn);
    double area = 0.;
    for (double s : y) area += s * dt;
    std::printf("%-10.0f %-10.3f %-9.4f %-11.1f %-10.4f %-9.1f %-9.2f\n", d, ppk, pk, tpk, mn, tmn, area);
    preAll.push_back(x);
    shapedAll.push_back(y);
  }
  for (size_t i = 0; i < t.size(); i += 4) {
    csv << t[i];
    for (size_t k = 0; k < preAll.size(); k++) csv << "," << preAll[k][i] << "," << shapedAll[k][i];
    csv << "\n";
  }
  std::printf("   (area -> 0 for a finite decay: the CR stage has no DC gain, so positive and negative lobes cancel;\n"
              "    for decay = 0 the area is just the running integral of a plateau and is not meaningful)\n");

  // (4) superposition with decay
  pa.SetDecayNs(300.);
  std::vector<Avalanche> two = {Avalanche(1., 0.), Avalanche(2., 40.)};
  std::vector<double> sumShaped = sh.Process(pa.Waveform(two), t);
  std::vector<double> a = sh.Process(pa.Waveform(two[0]), t), b = sh.Process(pa.Waveform(two[1]), t);
  double ms = 0.;
  for (size_t i = 0; i < t.size(); i++) ms = std::max(ms, std::fabs(sumShaped[i] - (a[i] + b[i])));
  std::printf("\n(4) shaping the sum vs summing the shaped pulses (decay 300): max difference = %.2e\n", ms);

  // (5) tauD == tauF
  pa.SetDecayNs(kTauRecharge);
  std::vector<double> xe = pa.Waveform(av);
  bool ok = true;
  for (double s : xe) if (!std::isfinite(s)) ok = false;
  std::printf("(5) tauD == tauF: finite = %s, peak = %.4f (expect exp(-1) = %.4f)\n", ok ? "yes" : "NO",
              *std::max_element(xe.begin(), xe.end()), std::exp(-1.));
  return 0;
}

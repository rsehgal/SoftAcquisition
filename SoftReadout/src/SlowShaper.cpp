/*
**	Filename : SlowShaper.cpp
**	2026-10-30
**	username : rsehgal
*/
#include "SlowShaper.h"
#include "Config.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

SlowShaper::SlowShaper()
{
  Update();
}

SlowShaper::~SlowShaper() {}

SlowShaper::SlowShaper(const Config &c) : cfg(c)
{
  Update();
}

void SlowShaper::Update()
{
  if (cfg.shapingIndex < 1 || cfg.shapingIndex > 7)
    throw std::invalid_argument("SlowShaper : shapingIndex must be between 1..7 (12.5..87.5)");

  n         = cfg.order > 0 ? cfg.order : slowShaperOrder;
  peakingNs = cfg.shapingIndex * slowShaperStepNs;
  tau       = peakingNs / n;
  norm      = std::tgamma(n + 1.) * std::exp((double)n) / std::pow((double)n, n); // 1 / (peak of x^n e^-x / n!)
}
double SlowShaper::PeakingTimeNs() const
{
  return peakingNs;
}
double SlowShaper::Tau() const
{
  return tau;
}
double SlowShaper::Norm() const
{
  return norm;
}
unsigned short SlowShaper::Order() const
{
  return n;
}

void SlowShaper::SetShapingIndex(unsigned short spIndex)
{
  cfg.shapingIndex = spIndex;
  Update();
}

void SlowShaper::SetOrder(unsigned short ord)
{
  cfg.order = ord;
  Update();
}

std::vector<double> SlowShaper::ApplyCR(const std::vector<double> &in, double dt) const
{
  std::vector<double> out(in.size());
  const double c = 2. * tau / dt;
  double xPrev = 0., yPrev = 0.;
  for (size_t k = 0; k < in.size(); k++) {
    double y = (c * (in[k] - xPrev) + (c - 1.) * yPrev) / (c + 1.);
    out[k]   = y;
    xPrev    = in[k];
    yPrev    = y;
  }
  return out;
}

std::vector<double> SlowShaper::ApplyRC(const std::vector<double> &in, double dt) const
{
  std::vector<double> out(in.size());
  const double c = 2. * tau / dt;
  double xPrev = 0., yPrev = 0.;
  for (size_t k = 0; k < in.size(); k++) {
    double y = (in[k] + xPrev + (c - 1.) * yPrev) / (c + 1.);
    out[k]   = y;
    xPrev    = in[k];
    yPrev    = y;
  }
  return out;
}

std::vector<double> SlowShaper::Process(const std::vector<double> &in, double dt) const
{
  std::vector<double> v = ApplyCR(in, dt);        // remove the plateau
  for (int k = 0; k < n; k++) v = ApplyRC(v, dt); // smooth, n times
  for (auto &s : v) s *= norm;                    // ideal step -> peak 1
  return v;
}

std::vector<double> SlowShaper::Process(const std::vector<double> &in, const std::vector<double> &timeVec) const
{
  if (timeVec.size() < 2) throw std::invalid_argument("SlowShaper: time axis needs at least 2 samples");
  return Process(in, timeVec[1] - timeVec[0]);
}

void SlowShaper::Print() const
{
  std::cout << "SlowShaper : CR-RC^" << n << ", peaking time " << peakingNs << " ns, tau " << tau << " ns, norm " << norm
            << std::endl;
}

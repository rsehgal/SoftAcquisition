/*
**	Filename : Preamp.cpp
**	2026-10-27
**	username : rsehgal
*/
#include <iostream>
#include "Preamp.h"
#include "Config.h"
#include "globals.h"
Preamp::Preamp() {}

Preamp::Preamp(const Config &cfg) : preampCfg(cfg)
{

  /*for (unsigned int i = 0; i < 600; i++) {
    timeVec.push_back(i);
  }*/

  SetTimeAxis(0,500);
  UpdateGain();
}

void Preamp::SetTimeAxis(double tFirst, double tLast, double dt )
{
  timeVec.clear();
  for (double t = tFirst - 500.; t < tLast + 250. + 6*87.5 ; t += dt) timeVec.push_back(t);
}

void Preamp::UpdateGain()
{
  if (preampCfg.fCIndex == 0) preampCfg.fCIndex = 1;
  if (preampCfg.fCIndex > fcIndexMax) preampCfg.fCIndex = fcIndexMax;
  double c_in_pf = (preampCfg.gType == HIGH_GAIN) ? hgCapacitance : lgCapacitance;
  double c_f_pf  = preampCfg.fCIndex * fcStep;
  gainFactor     = c_in_pf / c_f_pf;
}

double Preamp::Process(double charge_pc)
{
  double output_voltage = charge_pc * gainFactor;

  // Optional: Clamp to maximum linear output (e.g., 2.5 V max swing)
  const double max_output_v = 2.5;
  if (output_voltage > max_output_v) {
    output_voltage = max_output_v;
  }
  return output_voltage;
}

Preamp::~Preamp() {}

void Preamp::SetFCIndex(unsigned short fcIn)
{
  preampCfg.fCIndex = fcIn;
  UpdateGain();
}

void Preamp::SetGainType(GainType gt)
{
  preampCfg.gType = gt;
  UpdateGain();
}

std::vector<double> Preamp::Waveform(const Avalanche av)
{
  std::vector<double> q(timeVec.size(), 0.);

  for (unsigned int i = 0; i < timeVec.size(); i++) {
    if (av.arrivalTime < timeVec[i]) {
      q[i] = gainFactor * av.charge * (1 - std::exp(-(timeVec[i] - av.arrivalTime) / kTauRecharge));
    }
  }
  return q;
}

void Preamp::Print() const
{
  std::cout << "GainType : " << (preampCfg.gType == HIGH_GAIN ? "HIGH" : "LOW")
            << " :: feedbackCapacitance Index : " << preampCfg.fCIndex
            << " (Cf = " << preampCfg.fCIndex * fcStep * 1000. << " fF) :: gain = " << gainFactor << std::endl;
}

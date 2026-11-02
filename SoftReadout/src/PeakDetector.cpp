/*
**	Filename : PeakDetector.cpp
**	2026-11-01
**	username : rsehgal
*/
#include "PeakDetector.h"
#include <iostream>
#include <stdexcept>
PeakDetector::PeakDetector()
{
  state = State::OFF;
}

PeakDetector::PeakDetector(const Config &c) : cfg(c), state(State::OFF) {}

void PeakDetector::Print() const
{
  cfg.Print();
}

void PeakDetector::Reset() {}

bool PeakDetector::Process(double tSample, double vSample, bool trigger, Result &res)
{
  /*Algo :
  Check the state
    if state is OFF then sample should be ignore until or unless it is a trigger
    Now if it is NOT a Trigger, then simple ignore it.
    But if it is A Trigger, then set the state to SENSING and fill the trigger
  time in Result
  */
  if (state == State::OFF) {
    if (trigger) {
      state     = State::SENSING;
      res.tTrig = tSample;
      res.tHold = tSample + cfg.holdDelayNs;
      res.vHold = vSample;
      return false;
    } else {
      return false;
    }
  }
  if (state == State::SENSING) {
    if (tSample < res.tHold) {
      if (res.vHold < vSample) res.vHold = vSample;
    } else {
      state = State::HOLD;
      return true;
    }
  }
  if (state == State::HOLD) {
    if (tSample < (res.tHold + cfg.convertNs)) {
      return false;
    } else {
      state = State::OFF;
      return false;
    }
  }

  return false;
}

std::vector<PeakDetector::Result> PeakDetector::Process(const std::vector<double> &tVec,
                                                        const std::vector<double> &vVec, const std::vector<bool> trVec)
{
  if (tVec.size() != vVec.size() || tVec.size() != trVec.size())
    throw std::invalid_argument("PeakDetector::Process : tVec, vVec and trVec should be of same size...");

  std::vector<Result> resVec;
  Result current;
  for (size_t i = 0; i < tVec.size(); i++) {
    if (Process(tVec[i], vVec[i], trVec[i], current)) {
      resVec.push_back(current);
    }
  }

  return resVec;
}

/*
**	Filename : PeakDetector.h
**	2026-11-01
**	username : rsehgal
*/
#ifndef PeakDetector_h
#define PeakDetector_h

#include <iostream>

class PeakDetector {

public:
  enum class State { OFF, SENSING, HOLD };

  struct Config {
    double activationNs = 1;
    double holdDelayNs = 104;
    double convertNs = 10000.;
    double clockNs = 8.;

    void Print() const {
      std::cout << " Activation After Trigger : " << activationNs
                << " : HoldDelay : " << holdDelayNs
                << " : ConversionTime : " << convertNs
                << " : ClockConversion : " << clockNs << std::endl;
    }
  };

  struct Result{
    double tTrig=0.;
    double tHold=0.;
    double vHold=0.;
    bool truncated=false;
  };


  void Reset();

  PeakDetector();
  explicit PeakDetector(const Config &c);

  void Print() const;

  bool Process(double tSample, double vSample, bool trigger, Result& res);
private:
  Config cfg;
  State state = State::OFF;
};
#endif

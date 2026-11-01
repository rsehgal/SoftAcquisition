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

  void Reset();

  PeakDetector();
  explicit PeakDetector(const Config &c);

  void Print() const;

private:
  Config cfg;
};
#endif

/*
**	Filename : Preamp.h
**	2026-10-27
**	username : rsehgal
*/
#ifndef Preamp_h
#define Preamp_h

#include "globals.h"

class Preamp {

public:
  enum GainType { HIGH_GAIN, LOW_GAIN };

  struct Config {
    GainType gType         = HIGH_GAIN;
    unsigned short fCIndex = 50;
  };

  double Process(double charge);

  void SetTimeAxis(double tFirst, double tLast, double dt = 0.5);
  void SetFCIndex(unsigned short fcIn);
  void SetGainType(GainType gt);
  
  Preamp();
  ~Preamp();
  Preamp(const Config &cfg);

  void UpdateGain();
  void Print() const;
  std::vector<double> Waveform(const Avalanche av) ;
  std::vector<double> Waveform(const std::vector<Avalanche> avVec) ;

  std::vector<double> GetTimeVec () const {return timeVec;}
  
private:
  Config preampCfg;
  double gainFactor;
  std::vector<double> timeVec;
};

#endif

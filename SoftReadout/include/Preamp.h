/*
**	Filename : Preamp.h
**	2026-10-27
**	username : rsehgal
*/
#ifndef Preamp_h
#define Preamp_h

class Preamp {

public:
  enum GainType { HIGH_GAIN, LOW_GAIN };

  struct Config {
    GainType gType         = HIGH_GAIN;
    unsigned short fCIndex = 50;
  };

  double Process(double charge);

  void SetFCIndex(unsigned short fcIn);
  void SetGainType(GainType gt);
  
  Preamp();
  ~Preamp();
  Preamp(const Config &cfg);

  void UpdateGain();
  void Print() const;

private:
  Config preampCfg;
  double gainFactor;
};

#endif

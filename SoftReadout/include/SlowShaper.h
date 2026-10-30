/*
**	Filename : SlowShaper.h
**	2026-10-29
**	username : rsehgal
*/
#ifndef SlowShaper_h
#define SlowShaper_h
#include <vector>

class SlowShaper {

public:
  struct Config {
    unsigned short shapingIndex = 4;
    unsigned short order        = 2;
  };

  SlowShaper();
  ~SlowShaper();
  explicit SlowShaper(const Config &cfg);

  void Update();
  void SetShapingIndex(unsigned short spIndex);
  void SetOrder(unsigned short ord);

  double PeakingTimeNs() const;
  double Tau() const;
  unsigned short Order() const;
  double Norm() const;

  std::vector<double> ApplyCR(const std::vector<double> &in, double dt) const;
  std::vector<double> ApplyRC(const std::vector<double> &in, double dt) const;
  std::vector<double> Process(const std::vector<double> &in, double dt) const;
  std::vector<double> Process(const std::vector<double> &in, const std::vector<double> &timeVec) const;
  void Print() const;

private:
  Config cfg;
  // Some cached values
  double peakingNs = 50.;
  unsigned short n = 2;
  double tau       = 25.; // time constant of every stage = peakingNs / n
  double norm      = 1.;  // n! e^n / n^n : makes the peak for an ideal unit step equal to 1
};
#endif

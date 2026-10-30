/*
**	Filename : SlowShaper.h
**	2026-10-29
**	username : rsehgal
*/
#ifndef SlowShaper_h
#define SlowShaper_h

class SlowShaper{

public:
struct Config{
unsigned short shapingIndex=4;
unsigned short order=2;
};


SlowShaper();
~SlowShaper();
explicit SlowShaper(const Config &cfg );

private:
Config cfg;

};
#endif

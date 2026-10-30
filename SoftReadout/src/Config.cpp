/*
**	Filename : Config.cpp
**	2026-10-27
**	username : rsehgal
*/
#include<iostream>
#include "Config.h"

// All the numbers used here are taken from DT5202 datasheet/manual
double hgCapacitance=15.;
double lgCapacitance=1.5;
unsigned short fcIndexMax=63;
double fcStep=0.025;


//Slow shaper
double slowShaperStepNs = 12.5;
unsigned short slowShaperIndexMax=7;

unsigned short slowShaperOrder=2; //Assumption

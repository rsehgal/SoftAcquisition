/*
**	Filename : Test_Preamp.cpp
**	2026-10-27
**	username : rsehgal
*/
#include<iostream>
#include "Preamp.h"

int main(int argc, char *argv[]){
   Preamp::Config cfg;
   cfg.gType=Preamp::LOW_GAIN;
   cfg.fCIndex = 61;

   Preamp pa(cfg);
   pa.Print();

   std::cout << pa.Process(1.009) << std::endl;
  return 0;
}

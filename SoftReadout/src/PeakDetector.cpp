/*
**	Filename : PeakDetector.cpp
**	2026-11-01
**	username : rsehgal
*/
#include<iostream>
#include "PeakDetector.h"

PeakDetector::PeakDetector(){

}

PeakDetector::PeakDetector(const Config& c) : cfg(c){

}

void PeakDetector::Print() const {
cfg.Print();
}


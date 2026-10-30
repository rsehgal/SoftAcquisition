/*
**	Filename : ReadoutHelpers.cpp
**	2026-10-30
**	username : rsehgal
*/
#include "ReadoutHelpers.h"
#include <iostream>
#include <algorithm>
double MaxOf(const std::vector<double> &v, const std::vector<double> &t, double &tPeak)
{
  size_t k = std::max_element(v.begin(), v.end()) - v.begin();
  tPeak    = t[k];
  return v[k];
}

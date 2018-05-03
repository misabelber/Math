#pragma once
#include "matrixes.h"

// TODO: how to add the regularization... 
class Fitter{
 public:
  M corr;
  V corry;

  Number reg=0;
  Number wsum;
  int size;

  Fitter(){};
  Fitter(int pars);               // Initialize it
  void init(int pars);
  void add(V &pars,Number y,Number w=1.0);  // Add a new sample
  void add(V &pars,Number y,V &reg,Number w=1.0);  // Add a new sample with a vector of uncorrelated noise (for regularization)
  bool solve(V &x,Number prec=1e-6);
  void setReg(Number n){reg=n;}
};

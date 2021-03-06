#include "linear_fitter.h"



Fitter::Fitter(int pars){init(pars);}

void Fitter::init(int pars){
  ::init(corr,pars,pars);
  ::init(corry,pars);
  size=pars;
  wsum=0;
}

void Fitter::add(V &pars,Number y,Number w){
  wsum+=w;
  for(int i=0;i<size;i++)    corry[i]+=pars[i]*y*w;

  for(int i=0;i<size;i++){ 
    Number *dst=&(corr[i][0]);
    const Number o=pars[i];
    for(int j=0;j<size;j++)
      dst[j]+=o*pars[j]*w;
  }
}

void Fitter::add(V &pars,Number y,V &reg,Number w){
  wsum+=w;
  for(int i=0;i<size;i++) corry[i]+=pars[i]*y*w;

  for(int i=0;i<size;i++){ 
    Number *dst=&(corr[i][0]);
    const Number o=pars[i];
    for(int j=0;j<size;j++) dst[j]+=o*pars[j]*w;
    dst[i]+=w*reg[i];
  }
}


bool Fitter::solve(V &x,Number prec){
  M a=corr;
  V b=corry;

  for(int i=0;i<size;i++) b[i]/=wsum;
  
  for(int i=0;i<size;i++) 
    for(int j=0;j<size;j++)
      a[i][j]/=wsum;
 
  
  //Regularization
  for(int i=0;i<size;i++) a[i][i]+=reg;
  
  //  return ::solve(a,b,x,a.size());  // The :: is to force to use the global namespace due to an interference with the local one
  return ::solvecholesky(a,b,x);  // The :: is to force to use the global namespace due to an interference with the local one
}

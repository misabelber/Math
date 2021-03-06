#include "matrixes.h"
#include "assert.h"
#include "math.h"

#include "stdio.h"


Number mult(V &v1,V &v2){
  int n=v1.size();
  Number r=0;
  Number *p1=&v1[0];
  Number *p2=&v2[0];
  for(int i=0;i<n;i++) r+=p1[i]*p2[i];
  return r;
}


void mult(M &m,V &v,V &o){
  int n=m.size();
  o.reserve(n);
  o.clear();
  for(int i=0;i<n;i++) {o.push_back(mult(m[i],v));} 
}

void mult(V &v,Number f){
  int n=v.size();
  Number *p=&v[0];
  for(int i=0;i<n;i++) p[i]*=f;
}

void mult(M &a, M &b, M &c){

  const int acol = a[0].size();
  const int arow = a.size();
  const int bcol = b[0].size();
  const int brow = b.size();
  if (acol != brow) {fprintf(stderr,"Matrixes non multiplicable!\n"); return;}

  init(c,arow,bcol);
  
  for (int i=0; i<arow; i++){
    for (int j =0; j<bcol; j++){
      c[i][j] = 0;
      for (int k=0; k<acol; k++){

	c[i][j] += a[i][k]*b[k][j];

      }      
    }	 
  }
}

void add(V &a,V &b,V &o){
  int n=a.size();
  o.assign(n,0);
  Number *p1=&a[0];
  Number *p2=&b[0];
  Number *p3=&o[0];
  //  for(int i=0;i<n;i++) o[i]=a[i]+b[i];
  for(int i=0;i<n;i++) p3[i]=p1[i]+p2[i];
}

void add_(V &a,V &b){
  int n=a.size();
  Number *p1=&a[0];
  Number *p2=&b[0];
  //  for(int i=0;i<n;i++) a[i]+=b[i];
  for(int i=0;i<n;i++) p1[i]+=p2[i];
}

void add(M &A, M& B, M &C){
  int n = A.size(); const int nb = B.size();
  int m = A[0].size(); const int mb = B[0].size();
  if (n != nb || m != mb){printf("Error! Matrixes of different dimension!"); return;}

  for (int i=0; i<n; i++){
    for (int j=0; j<m; j++){
      
      C[i][j] = A[i][j] + B[i][j];

    }
  }

}


void init(V &v,const int size){
  //v.assign(size,0);
  v.clear();
  for (int i=0; i<size; i++) v.push_back(0.0);
}

void init(M &m,const int rows,const int columns){
  V v;
  init(v,columns);
  for (int i=0; i<rows; i++) m.push_back(v);
  //m.assign(rows,v);
}
 
void add_col(M &m){
  const int n=m.size();
  for(int i=0;i<n;i++) m[i].push_back(0);
}

void add_row(M &m){
  V v;
  init(v,m[0].size());
  m.push_back(v);
}

void normalize(V &v){
  Number n=mult(v,v);
  if(n==0) return;
  mult(v,1.0/sqrt(n));
}


void rotZ(V &v,Number rad){
  Number c=cos(rad);
  Number s=sin(rad);
  
  Number i=c*v[0]-s*v[1];
  Number j=s*v[0]+c*v[1];
  v[0]=i;
  v[1]=j;
}

void rotY(V &v,Number rad){
  Number c=cos(rad);
  Number s=sin(rad);
  
  Number i=c*v[0]+s*v[2];
  Number k=-s*v[0]+c*v[2];
  v[0]=i;
  v[2]=k;
}

void transpose(M &a, M &b){

  const int acol = a[0].size();
  const int arow = a.size();
  init(b,acol,arow);
  for (int i=0; i<acol; i++){
    for (int j=0; j<arow; j++){
      
      b[i][j] = a[j][i];
      
    }
  }  
}

/*int cholesky(M &a, M &c){

  M b; //Me calcula una upper triangular
  const int n = a.size();
  init(b,n,n);
  
  for ( int i= 0; i<n; i++){
    b[i][i] = a[i][i];
    for (int k = 0; k< i; k++){
      b[i][i] -= b[k][i]*b[k][i];
      if (b[i][i] < 0){
	fprintf(stderr,"\nERROR: non-positive definite matrix!\n");
	printf("\nproblem from %d %f\n",i,b[i][i]);
	return 1;
      }
      
    }
    b[i][i] = sqrt(fabs(b[i][i]));
    
    for (int j = i+1; j < n; j++){
      b[i][j] = a[i][j];
      for (int k = 0; k < i; k++){
	
	b[i][j] -= b[k][i]*b[k][j]; 
      }
      b[i][j] /= b[i][i];
    }
    }

  transpose(b,c); //Para que sea lower triangular.
  return 0;
  }*/

void invert(M &A, M &invA){

  int n=A.size();
  M I;
  init(I,n,n);
  
  for (int i=0; i<n; i++){
    I[i][i] = 1;
  }
  init(invA,n,n);
 
  for (int i=0; i<n; i++){
    
    solve(A,I[i],invA[i],1e-15);
    
  }

  M test;
  mult(A,invA,test);
  double sum=0;
  for(int i=0; i<n; i++){
    for (int j=0; j<n; j++){
      sum += test[i][j] -I[i][j];      
    }    
  }
  
  if (sum > 1e-6) fprintf(stderr,"Matrix no invertible! Sum: %lf",sum); 

}

double solve(M &m,V &b,V &x,Number prec, bool print){
  V r0,p0,x0;
  init(r0,m[0].size());
  init(x0,m[0].size());

  mult(m,x0,r0);
  mult(r0,-1);
  add_(r0,b);
  p0=r0;

  prec*=prec;
  double ldet=0;
  //  while(1){
  for(int i=0;i<2*m.size();i++){
    Number alpha=mult(r0,r0);
    V temp;
    mult(m,p0,temp);

    Number d=mult(p0,temp);
    if(d==0) return 1;
    alpha/=d;
    
    Number r_norma=mult(r0,r0);
    if(r_norma==0) return 1;

    //Update temp vars
    mult(p0,alpha);
    add_(x0,p0);

    mult(temp,-alpha);
    add_(r0,temp);

    printf("NOW %f\n",sqrt(mult(r0,r0)));

    if(mult(r0,r0)<prec) break;

    Number beta=mult(r0,r0)/r_norma;

    mult(p0,beta/alpha);
    add_(p0,r0);
    if (print == true) {
      printf("NOW %f\n",sqrt(mult(r0,r0)));}
  }
  
  x=x0;
  return ldet;
  
}

double solvepositive(M &m,V &b,V &x,Number prec, bool print){
  V r0,p0,x0,grad;
  init(r0,m[0].size());
  init(x0,m[0].size());

  mult(m,x0,r0);
  mult(r0,-1);
  add_(r0,b);

  grad = r0;
  mult(grad,-1);
  

  p0=r0;

  for (int i=0; i<x0.size(); i++) {
    
    if (x0[i]<=0){
      if (grad[i]>0) p0[i] = 0;
      else p0[i] = r0[i];
    }

    if (x0[i]>0){
      p0[i] = r0[i];
    }
  }

  prec*=prec;
  double ldet=0;
  //  while(1){
  for(int i=0;i<2*m.size();i++){
    Number alpha=mult(r0,r0);
    V temp;
    mult(m,p0,temp);

    Number d=mult(p0,temp);
    if(d==0) return 1;
    alpha/=d;
    
    Number r_norma=mult(r0,r0);
    if(r_norma==0) return 1;

    //Update temp vars

    mult(p0,alpha);
    add_(x0,p0);

    mult(temp,-alpha);
    add_(r0,temp);
    
     

    printf("NOW %f\n",sqrt(mult(r0,r0)));

    if(mult(r0,r0)<prec) break;

    grad=r0;
    mult(grad,-1);

    Number beta=mult(r0,r0)/r_norma;

    for (int ii=0; ii<x0.size(); ii++) {
      
      if (x0[ii]==0){
	if (grad[ii]>0) {p0[ii] = 0; }
	else p0[ii] = r0[ii];
      }
      
      else if(x0[ii]>0){
	p0[ii] = r0[ii]+p0[ii]*beta/alpha;
      }
    }
   
    if (print == true) {
      printf("NOW %f\n",sqrt(mult(r0,r0)));}
  }
  
  x=x0;
  return ldet;

}



bool my_solve(M &m,V &b,V &x,Number prec){
  
  int pars = b.size();

  V r; init(r,pars);
  V p; init(p,pars);

  double rsold; 
  double rsnew;
  double alpha;

  mult(m,x,r);
  mult(r,-1);
  add_(r,b);
  p = r;
  rsold=mult(r,r);

  for (int i=0; i<2*pars; i++){
    V Ap;
    mult(m,p,Ap);
    double tmp;
    tmp=mult(p,Ap);
    alpha = rsold/tmp;
    mult(p,alpha);
    add_(x,p);
    mult(Ap,alpha);
    mult(Ap,-1);
    add_(r,Ap);
    //if (i%1000==0) cout << sqrt(rsnew) << "\t" << ch << endl;
    rsnew=mult(r,r);
    if (sqrt(rsnew) < 1e-4) break;
    mult(p,rsnew/rsold/alpha);
    add_(p,r);
    rsold = rsnew;   
   
        
  };
   printf("%f\n", sqrt(mult(r,r)));
   
   return false;
}

void cholesky(M &m,M &l){
  int n=m.size();
  init(l,n,n);

  for (int i = 0; i < n; i++)
    for (int j = 0; j < (i+1); j++) {
      Number s = 0;
      for (int k = 0; k < j; k++) s += l[i][k] * l[j][k];
      l[i][j]= (i == j) ? sqrt(fabs(m[i][i] - s)) : (1.0 / l[j][j] * (m[i][j] - s));
    }
}

void solvell(M &l,V a,V &solution){
  init(solution,l.size());
  int n=l.size();

  for(int i=0;i<n;i++){
    solution[i]=a[i]/l[i][i];
    for(int j=i;j<n;j++) 
      a[j]-=l[j][i]*solution[i];
      
  }
}


void solvelt(M &l,V a,V &solution){
  init(solution,l.size());
  int n=l.size();

  // Build the transpose
  M lt;
  init(lt,n,n);
  for(int i=0;i<n;i++)
    for(int j=0;j<=i;j++)
      lt[j][i]=l[i][j];

  for(int i=n-1;i>=0;i--){
    solution[i]=a[i]/lt[i][i];
    for(int j=i;j>=0;j--) 
      a[j]-=lt[j][i]*solution[i];
  }
  
}

#include <iostream>
Number solvecholesky(M &m,V &b,V &x,Number &logdet){
  M l;
  cholesky(m,l);

  // Get the determinant
  logdet=0;
  for(int i=0;i<l.size();i++) logdet+=2*log(l[i][i]);

  // Solve the system
  V sol1;
  solvell(l,b,sol1);
  solvelt(l,sol1,x);
   

  return 0;
}


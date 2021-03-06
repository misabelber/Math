#pragma once
#include <vector>


/****************************************************************
Matrix and vector stuff:
- vector are represented with an array
- matric is a vector of vector
- operations are done on the array itself
 ****************************************************************/


typedef double Number;
typedef std::vector<Number> V;  // Vector
typedef std::vector<V> M;       // Matrx
typedef std::vector<M> VM;     // Vector of Matrixes



void init(V &v,const int size);                   // Initialize a vector
void init(M &m,const int rows,const int columns); // Initialize a matrix


void mult(M &v1,V &v2,V &o); // Matrix product
Number mult(V &v1,V &v2);     // Scalar product
void mult(V &v,Number n);    // Vector scaling
void mult(M &a, M &b, M &c);

void add(V &a,V &b,V &o);
void add_(V &a,V &b); // Add in place
void add(M &A, M& B, M &C);

void add_col(M &m); // Growing in columns
void add_row(M &m); // Growing in rows

void normalize(V &v);

// Sepecial operators for rotations
void rotZ(V &v,Number rad);
void rotY(V &v,Number rad);

void transpose(M &a, M &b);

//int cholesky(M &a, M &c);

void invert(M &A, M &invA);
void cholesky(M &m,M &l);
void solvell(M &m,V a, V &solution);
void solvelt(M &m,V a, V &solution);
Number solvecholesky(M &m,V &b,V &x,Number &logdet); // Conjugate gradient solvinf of mx=b. Returns the solution and the log |det|
Number solvecholesky(M &m,V &b,V &x){Number dummy;return solvecholesky(m,b,x,dummy);} // Conjugate gradient solvinf of mx=b.

// Matrix solution
double solve(M &m,V &b,V &x, Number prec=1e-6, bool print=false); // Conjugate gradient solvinf of mx=b. Return 1 if failed
double solvepositive(M &m,V &b,V &x, Number prec=1e-6, bool print=false);
bool my_solve(M &m,V &b,V &x,Number prec=1e-6);
  




#ifndef SLEQ_H
#define SLEQ_H

#include "tMatrix.h"
#include "tVector.h"

class tSLEQ
{
protected:
   tMatrix *basicM_p, *bM_p;
   int n, _b_cnt;

public:
   tSLEQ(const tMatrix &Basic, const tVector &B); // Basic matrix must be square.
   tSLEQ(const tMatrix &Basic, const tMatrix &B);

   void Hauss(tMatrix &Result);
   void Hauss(tVector &Result);
   void LU(tMatrix &Result);  // Is not ready.
   void LU(tVector &Result);
   void RootMeth(tVector &Result);

   static tVector Solve_Left(const tMatrix &L, const tVector &B);
   static tVector Solve_Right(const tMatrix &R, const tVector &B);
   tVector Solve_D(const tMatrix &D, const tVector &B);
   
   ~tSLEQ();
};

#endif

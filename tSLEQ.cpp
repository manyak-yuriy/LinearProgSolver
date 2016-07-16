#include "tSLEQ.h"
#include "globals.h"
#include "tVector.h"
#include <iostream>
#include "tMatrix.h"

#include <math.h>

tSLEQ :: tSLEQ(const tMatrix &Basic, const tVector &B)
{
   if ( Basic.col_cnt() != Basic.row_cnt() )
	  throw "SLEQ basic matrix must be square.";
   if ( B.size() != Basic.row_cnt() )
	  throw "SLEQ right part dimension doesn't fit basic matrix.";

   this -> n = B.size();
   this -> _b_cnt = 1;

   basicM_p = new tMatrix(Basic);
   bM_p = new tMatrix(this -> n, 1);
   for (int i = 0; i < this -> n; i++)
	  (*bM_p)(i, 0) = B.elem(i);
}

tSLEQ :: tSLEQ(const tMatrix &Basic, const tMatrix &B)
{
   if ( Basic.col_cnt() != Basic.row_cnt() )
	  throw "SLEQ basic matrix must be square.";
   if ( B.row_cnt() != Basic.row_cnt() )
	  throw "SLEQ right part dimension doesn't fit basic matrix.";

   this -> n = B.row_cnt();
   this -> _b_cnt = B.col_cnt();

   basicM_p = new tMatrix(Basic);
   bM_p = new tMatrix(B);
}

tSLEQ :: ~tSLEQ()
{
   delete basicM_p;
   delete bM_p;
   basicM_p = 0;
   bM_p = 0;
   n = 0;
   _b_cnt = 0;
}

void tSLEQ :: Hauss(tMatrix &Result)
{
   if ( Result.row_cnt() != n || Result.col_cnt() != _b_cnt )
	  throw "SLEQ Hauss method: Argument does't fit result format.";
   
   tMatrix *extM_p = new tMatrix(n, n + _b_cnt);
   for (int i = 0; i < n; i++)
	  for (int j = 0; j < n; j++)
		 (*extM_p)(i, j) = (*basicM_p)(i, j);

   for (int i = 0; i < n; i++)
	  for (int j = 0; j < _b_cnt; j++)
		 (*extM_p)(i, j + n) = (*bM_p)(i, j);

  // extM_p -> HaussStraight();
   extM_p -> HaussBack();

   for (int i = 0; i < n; i++)
	  for (int j = 0; j < _b_cnt; j++)
		 Result(i, j) = (*extM_p)(i, j + n);
   delete extM_p;
}

void tSLEQ :: Hauss(tVector &Result)
{
   tMatrix *ResM_p = new tMatrix(n, _b_cnt);
   Hauss(*ResM_p);
   Result = ResM_p -> get_col(0);
   delete ResM_p;
}

void tSLEQ :: LU(tMatrix &Result)
{
   throw "Function is under construction.";
}

tVector tSLEQ :: Solve_Left(const tMatrix &L, const tVector &B)
{
   int n = B.size();
   if ( n != L.col_cnt() || n != L.row_cnt() )
	  throw "Can't solve equation with left basic matrix: dimensions doesn't fit each other.";
   tVector X( n );
   for (int iX = 0; iX < n; iX++)
   {
	  X[iX] = B.elem(iX);
	  for (int j = 0; j < iX; j++)
		 X[iX] -= L.at(iX, j) * X.elem(j);
	  if ( is_zero(L.at(iX, iX)) )
		 throw "can't solve L-matrix: zero on diagonal.";
	  X[iX] /= L.at(iX, iX);
   }
   return X;
}

tVector tSLEQ :: Solve_Right(const tMatrix &R, const tVector &B)
{
   int n = B.size();
   if ( n != R.col_cnt() || n != R.row_cnt() )
	  throw "Can't solve equation with right basic matrix: dimensions doesn't fit each other.";
   tVector X( n );
   for (int iX = n - 1; iX >= 0; iX--)
   {
	  X[iX] = B.elem(iX);
	  for (int j = iX + 1; j < n; j++)
		 X[iX] -= R.at(iX, j) * X.elem(j);
	  if ( is_zero(R.at(iX, iX)) )
		 throw "can't solve R-matrix: zero on diagonal.";
	  X[iX] /= R.at(iX, iX);
   }
   return X;
}

void tSLEQ :: LU(tVector &Result)
{
   // L(Ux) = b    <->   Ly = b    ->    y

   tVector B = bM_p -> get_col(0);
   tMatrix L(*basicM_p);
   L.Turn_L();
   tVector Y = Solve_Left(L, B);

   // Ux = y    ->    x
   tMatrix U(*basicM_p);
   U.Turn_U();
   tVector X = Solve_Right(U, Y);

   Result = X;
}

void tSLEQ :: RootMeth(tVector &Result)
{
   if ( ! basicM_p -> is_symm() )
	  throw "Square root method is not acceptable: Basic matrix is not symmetric.";
   if ( basicM_p -> row_cnt() != basicM_p -> col_cnt())
      throw "Root Method: Matrix must be square.";

   tMatrix A( *basicM_p );
   tVector b( bM_p -> get_col(0) );
   tMatrix A_tr( A );

   A_tr.trans();

   int n = A.row_cnt();
   tMatrix L(n, n), D(n, n);
   A.Get_DLMatr(D, L);
   D.print("D-matrix: ", "D.txt");
   L.print("L-matrix: ", "L.txt");

   // L * ( D * L_tr (x) ) = b  <->
   // L(y) = b   ->   y

   tVector Y = Solve_Left(L, b);
   Y.print("Y-vector: ", "Y.txt");
   // D * (L_tr) (x) = y   <->   L_tr(x) = z    ->   z

   tVector Z = Solve_D(D, Y);
   Z.print("Z-vector: ", "Z.txt");
   tMatrix L_tr(L);
   L_tr.trans();

   //  L_tr (x) = z   ->   x
   tVector X = Solve_Right(L_tr, Z);
   X.print("X-vector: ", "X.txt");
   Result = X;
}

tVector tSLEQ :: Solve_D(const tMatrix &D, const tVector &B)
{
   int n = B.size();
   if ( n != D.col_cnt() || n != D.row_cnt() )
	  throw "Can't solve equation with left basic matrix: dimensions don't fit each other.";
   tVector X(n);
   for (int iX = 0; iX < n; iX++)
   {
	  if ( is_zero( D.at(iX, iX) ) )
		 throw "Can't solve D-matrix: zero on diagonal.";
	  X[iX] = B.elem(iX) / D.at(iX, iX);
   }
   return X;
}

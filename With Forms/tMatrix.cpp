#include <iostream.h>
#include <fstream.h>
#include <math.h>
#include <iomanip.h>

#include "globals.h"
#include "tMatrix.h"
#include "tVector.h"


template <class T>
inline int min(T a, T b)
{
   if (a > b)
	  return b;
   else
      return a;
}

tMatrExc :: tMatrExc(const char *msg)
{
   Msg_p = new string(msg);
}

const char * tMatrExc :: message() const
{
   return Msg_p -> c_str();
}

tMatrExc :: ~tMatrExc()
{
   delete Msg_p;
   Msg_p = 0;
}

tMatrix :: tMatrix(int _row_cnt, int _col_cnt)
{
   arr = new double[ _row_cnt * _col_cnt ];
   if (arr == 0)
	  throw new tMatrExc("Memory error.");
   this -> _row_cnt = _row_cnt;
   this -> _col_cnt = _col_cnt;
   for (int i = 0; i < _row_cnt; i++)
	  for (int j = 0; j < _col_cnt; j++)
		 (*this)(i, j) = 0;
}

tMatrix :: tMatrix(const tMatrix &Matr)
{
   this -> _row_cnt = Matr.row_cnt();
   this -> _col_cnt = Matr.col_cnt();
   this -> arr = new double[ _row_cnt * _col_cnt ];
   for (int i = 0; i < _row_cnt; i++)
	  for (int j = 0; j < _col_cnt; j++)
		 (*this)(i, j) = Matr.at(i, j);
}

tMatrix :: tMatrix(const tMatrix &Matr, tVector &Vec)
{
   this -> _row_cnt = Matr.row_cnt();
   this -> _col_cnt = Matr.col_cnt() + 1;
   this -> arr = new double[ _row_cnt * _col_cnt ];
   for (int i = 0; i < _row_cnt; i++)
	  for (int j = 0; j < _col_cnt - 1; j++)
		 (*this)(i, j) = Matr.at(i, j);
   for (int i = 0; i < _row_cnt; i++)
      (*this)(i, _col_cnt - 1) = Vec[i];
}

tMatrix :: tMatrix(const char *filename)
{
   ifstream file(filename);
   file >> _row_cnt;
   file >> _col_cnt;

   arr = new double[ _row_cnt * _col_cnt ];
   if (arr == 0)
	  throw new tMatrExc("Memory error.");

   for (int i = 0; i < _row_cnt; i++)
	  for (int j = 0; j < _col_cnt; j++)
		 file >> (*this)(i, j);
}

void tMatrix :: trans()
{
   int n = ::min<int>(_row_cnt, _col_cnt);
   for (int i = 0; i < n; i++)
	  for (int j = 0; j < i + 1; j++)
	  {
		 double t = at(i, j);
		 (*this)(i, j) = at(j, i);
		 (*this)(j, i) = t;
      }
}

double & tMatrix :: operator()(int row, int col)
{
   if ( row >= _row_cnt || col >= _col_cnt )
	  throw new tMatrExc("Matrix index is out of range.");
   return arr[col + row * _col_cnt];
}

double tMatrix :: at(int row, int col) const
{
   if ( row >= _row_cnt || col >= _col_cnt )
	  throw new tMatrExc("Matrix index is out of range.");
   return arr[col + row * _col_cnt];
}


int tMatrix :: is_symm() const
{
   int n = ::min<int>(_row_cnt, _col_cnt);
   for (int i = 0; i < n; i++)
	  for (int j = 0; j < i + 1; j++)
		 if ( at(i, j) != at(j, i) )
			return 0;
   return 1;
}


void tMatrix :: row_add(int first, int second, double koef = 1)
{
   for (int j = 0; j < _col_cnt; j++)
	  (*this)(first, j) += (*this)(second, j) * koef;
}

void tMatrix :: col_add(int first, int second, double koef = 1)
{
   for (int i = 0; i < _row_cnt; i++)
	  (*this)(i, first) += (*this)(i, second) * koef;
}

void tMatrix :: row_mul(int row, double k)
{
   for (int j = 0; j < _col_cnt; j++)
	  (*this)(row, j) *= k;
}

void tMatrix :: col_mul(int col, double k)
{
   for (int i = 0; i < _row_cnt; i++)
	  (*this)(i, col) *= k;
}

tMatrix :: ~tMatrix()
{
   _row_cnt = 0;
   _col_cnt = 0;
   if (arr)
	  delete[] arr;
   arr = 0;
}

void tMatrix :: print(const char *hint, const char *filename) const
{
   int dig;
   int max_len = 0;

   for (int i = 0; i < _row_cnt; i++)
	  for (int j = 0; j < _col_cnt; j++)
		 if ( ( dig = len(at(i, j)) ) > max_len  )
			max_len = dig;

   if (max_len > MAX_WDTH)
	  max_len = MAX_WDTH;
   ofstream file(filename);
   double num;
   file << hint << endl;
   for (int i = 0; i < _row_cnt; i++)
   {
	  for (int j = 0; j < _col_cnt; j++)
	  {
		 num = at(i, j);
		 /*if ( abs(num) < EPS )
			num = 0;     */
		 file << setw(max_len + PREC + 1) << setprecision(PREC) << num << ' ';
	  }
	  file << endl;
   }

}

void tMatrix :: row_swap(int first, int second)
{
   if ( first < 0 || first >= _row_cnt || second < 0 || second >= _row_cnt )
	  throw new tMatrExc("Reference to unexisting row.");
   for (int j = 0; j < _col_cnt; j++)
   {
	  double t = at(first, j);
	  (*this)(first, j) = at(second, j);
	  (*this)(second, j) = t;
   }
}

void tMatrix :: col_swap(int first, int second)
{
   if ( first < 0 || first >= _col_cnt || second < 0 || second >= _col_cnt )
	  throw new tMatrExc("Reference to unexisting col.");
   for (int i = 0; i < _row_cnt; i++)
   {
	  double t = at(i, first);
	  (*this)(i, first) = at(i, second);
	  (*this)(i, second) = t;
   }
}

#include <iostream>
int tMatrix :: HaussStraight(int var[], int var_cnt)
{
   int n = ::min<int>(_row_cnt, _col_cnt);

   for (int step = 0; step <= n - 2; step++)
   {
      /*
	  int major = step;
	  for (int i = step + 1; i < _row_cnt; i++)
		 if ( at(i, step) > at(major, step) )
			major = i;
	  if ( is_zero(at(major, step)) )
	     throw new tMatrExc("Matrix is degenerate.");
	  else
		 row_swap(major, step);  */
	  if ( is_zero(at(step, step)) )
	  {
		  int found = 0;
		  for (int j = step + 1; j < _col_cnt; j++)
		  if ( !is_zero(at(step, j)) )
		  {
			 found = 1;
			 col_swap(step, j);
			 int t = var[step + 1];
			 var[step + 1] = var[j + 1];
			 var[j + 1] = t;
			 break;
          }
		  if (! found)
		     return -1;
      }
	  for (int i = step + 1; i < _row_cnt; i++)
		 row_add(i, step, -at(i, step) / at(step, step));
	  //cout << *this;
   }
   return 0;
}

void tMatrix :: HaussBack(void)
{
   int n = ::min<int>(_row_cnt, _col_cnt);
   for (int step = n - 1; step > 0; step--)
   {
	  if ( is_zero( at(step, step) ) )
		 throw new tMatrExc("Matrix is degenerate.");
	  for (int i = step - 1; i >= 0; i--)
		 row_add(i, step, -at(i, step)/at(step, step) );
   }
   for (int i = 0; i < n; i++)
	  row_mul(i, 1 / at(i, i));
}

double tMatrix :: Invert()
{
   if (_col_cnt != _row_cnt)
	  throw new tMatrExc("Non-square Matrix can't be inverted.");
   int n = _row_cnt;
   tMatrix Mtr(n, 2 * n);

   for (int i = 0; i < n; i++)
   {
	  for (int j = 0; j < n; j++)
		 Mtr(i, j) = (*this)(i, j);
	  Mtr(i, n + i) = 1;
   }
   
   //Mtr.HaussStraight();
   double detA = 1;
   for (int i = 0; i < n; i++)
	  detA *= Mtr.at(i, i);
   Mtr.HaussBack();

   for (int i = 0; i < n; i++)
	  for (int j = 0; j < n; j++)
		 (*this)(i, j) = Mtr(i, j + n);

   return detA;
}

void tMatrix :: Turn_U(void)
{
   //HaussStraight();
}

void tMatrix :: Turn_L(void)
{
   trans();
   //HaussStraight();
   trans();
   for (int i = 0; i < _row_cnt; i++)
	  col_mul(i, 1 / at(i, i));
}

tVector tMatrix :: get_col(int col) const
{
   if ( col >= _col_cnt )
	  throw new tMatrExc("Coloumn index out of range.");
   tVector V(_row_cnt);
   for (int i = 0; i < _row_cnt; i++)
	  V[i] = at(i, col);
   return V;
}

tVector tMatrix :: get_row(int row) const
{
   if ( row >= _row_cnt )
	  throw new tMatrExc("Row index out of range.");
   tVector V(_col_cnt);
   for (int i = 0; i < _col_cnt; i++)
	  V[i] = at(row, i);
   return V;
}

tVector operator*(const tMatrix &Mat, const tVector &Vec)
{
   if ( Mat.col_cnt() != Vec.size() )
	  throw "Can't multiplicate Matrix on vector.";
   tVector ResV(Mat.row_cnt());
   for (int row = 0; row < Mat.row_cnt(); row++)
	  ResV[row] = Mat.get_row(row) * Vec;
   return ResV;
}

tMatrix operator*(const tMatrix &Mat1, const tMatrix &Mat2)
{
   if ( Mat1.col_cnt() != Mat2.row_cnt() )
	  throw "Not appropriate dimensions of matrixes to multiply.";
   tMatrix Mat( Mat1.row_cnt(), Mat2.col_cnt() );

   for (int row = 0; row < Mat1.row_cnt(); row++)
	  for (int col = 0; col < Mat2.col_cnt(); col++)
		 Mat(row, col) = Mat1.get_row(row) * Mat2.get_col(col);

   return Mat;
}

void tMatrix :: Get_DLMatr(tMatrix &D, tMatrix &L)
{
   tMatrix A(*this);
   if ( A.row_cnt() != A.col_cnt() )
	  throw "Matrix must be square to retrieve an L matrix.";
   int n = A.row_cnt();

   for (int k = 0; k < n; k++)
   {
	  double expr = A.at(k, k);
	  for (int i = 0; i <= k - 1; i++)
		 expr -= D.at(i, i) * sqr( L.at(k, i) );
	  if ( (D(k, k) = sign( expr )) == 0 )
		 throw "A zero got on the diagonal of D-matrix: method is unacceptable.";
	  L(k, k) = D(k, k) * sqrt( abs(expr) );
	  if ( is_zero( L.at(k, k) ) )
	     throw "Got a zero on diagonal of L-matrix.";
	  for (int j = k + 1; j < n; j++)
	  {
		  double sum = A.at(j, k);
		  for (int i = 0; i <= k - 1; i++)
			sum -= D.at(i, i) * L.at(k, i) * L.at(j, i);
		  sum /= ( D.at(k, k) * L.at(k, k) );
		  L(j, k) = sum;
      }
   }
}
	 /*
double tMatrix :: minor(int i_start, int i_fin, int j_start, int j_fin, int check = 1)
{
   if (check)
   {
	  if (i_start >= _row_cnt || i_start < 0)
		 throw "i_start index index if out of range";
	  if (i_fin >= _row_cnt || i_fin < 0)
		 throw "i_finish index index if out of range";
	  if (j_start >= _col_cnt || j_start < 0)
		 throw "j_start index index if out of range";
	  if (j_fin >= _col_cnt || j_fin < 0)
		 throw "j_finish index index if out of range";

	  if ( i_fin < i_start )
		 throw "Incorrect order of minor rows.";
	  if ( j_fin < j_start )
		 throw "Incorrect order of minor coloumns.";

	  if ( i_fin - i_start != j_fin - j_start )
		 throw "Minor must be referred to a square matrix.";
   }
   double result = 0;

   return result;
}    */

int tMatrix :: isSquare(void) const
{
   if ( this->_row_cnt == this->_col_cnt )
	  return 1;
   return 0;
}

tMatrix & tMatrix :: operator=(const tMatrix &M)
{
   if ( M.row_cnt() > this -> row_cnt() || M.col_cnt() > this -> col_cnt() )
   {
	  delete[] arr;
	  arr = new double[M.row_cnt() * M.col_cnt()];
	  this -> _row_cnt = M.row_cnt();
	  this -> _col_cnt = M.col_cnt();
   }
   for (int i = 0; i < M.row_cnt(); i++)
	  for (int j = 0; j < M.col_cnt(); j++)
		 (*this)(i, j) = M.at(i, j);
   for (int i = M.row_cnt(); i < this -> row_cnt(); i++)
	  for (int j = M.col_cnt(); j < this -> col_cnt(); j++)
	     (*this)(i, j) = 0;
   return *this;
}

int tMatrix :: diag_pref(void) const
{
   int n = ::min<int>(this -> row_cnt(), this -> col_cnt());
   for (int i = 0; i < n; i++)
   {
	  double sum = 0;
	  for (int j = 0; j < i; j++)
		 sum += abs( this -> at(i, j) );
	  for (int j = i + 1; j < n; j++)
		 sum += abs( this -> at(i, j) );
	  if ( abs(at(i, i)) < sum )
	     return 0;
   }
   return 1;
}

tMatrix tMatrix :: SubMatr(int row_1, int col_1, int row_2, int col_2) const
{
   if ( row_1 < 0 || row_1 >= this -> row_cnt() )
	  throw "row_1 is out of range.";
   if ( col_1 < 0 || col_1 >= this -> col_cnt() )
	  throw "col1_ is out of range.";
   if ( row_2 < 0 || row_2 >= this -> row_cnt() )
	  throw "row_2 is out of range.";
   if ( col_2 < 0 || col_2 >= this -> col_cnt() )
	  throw "col_2 is out of range.";
   if ( col_1 > col_2 || row_1 > row_2 )
	  throw "Submatrix index order is incorrect.";
   
   tMatrix M(row_2 - row_1 + 1, col_2 - col_1 + 1);
   for (int i = row_1; i <= row_2; i++)
	  for (int j = col_1; j <= col_2; j++)
	     M(i, j) = this -> at(i, j);
   return M;
}


ostream &operator<<(ostream &file, tMatrix M)
{
   int dig;
   int max_len = 0;

   for (int i = 0; i < M.row_cnt(); i++)
	  for (int j = 0; j < M.col_cnt(); j++)
		 if ( ( dig = len(M.at(i, j)) ) > max_len )
			max_len = dig;

   if (max_len > MAX_WDTH)
	  max_len = MAX_WDTH;
   double num;

   for (int i = 0; i < M.row_cnt(); i++)
   {
	  for (int j = 0; j < M.col_cnt(); j++)
	  {
		 num = M.at(i, j);
		 /*if ( abs(num) < EPS )
			num = 0;     */
		 file << setw(max_len + PREC + 1) << setprecision(PREC) << num << ' ';
	  }
	  file << endl;
   }
   file << '\n';

   return file;
}

void tMatrix :: canon(void)
{
   _row_cnt--;
   for (int step = _row_cnt - 1;  step >= 0; step--)
   {
	  row_mul(step, 1 / at(step, step));
	  for (int i = step - 1; i >= 0; i--)
		 row_add(i, step, -at(i, step));
	  //cout << *this;
   }
   _row_cnt++;   
}

void tMatrix :: append_row(const tVector Vec)
{
   tMatrix temp = *this;
   delete[] arr;
   arr = new double[ (_row_cnt + 1) * _col_cnt ];
   if (arr == 0)
	  throw new tMatrExc("Memory error.");
   this -> _row_cnt = _row_cnt + 1;
   this -> _col_cnt = _col_cnt;
   for (int i = 0; i < _row_cnt - 1; i++)
	  for (int j = 0; j < _col_cnt; j++)
		 (*this)(i, j) = temp.at(i, j);
   for (int j = 0; j < _col_cnt; j++)
      (*this)(_row_cnt - 1, j) = Vec.elem(j);
}

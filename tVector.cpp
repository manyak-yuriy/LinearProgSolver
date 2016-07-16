#include "tVector.h"
#include <fstream.h>
#include "globals.h"
#include <iomanip.h>

tVector :: tVector(int n)
{
   if (n <= 0)
	  throw "Can''t create a zero-dimension vector!";
   this -> n = n;
   v = new double[n];
   for (int i = 0; i < n; i++)
      v[i] = 0;
}

tVector operator*(double k, const tVector &Vec)
{
   tVector Res(Vec);
   for (int i = 0; i < Vec.size(); i++)
      Res[i] *= k;
   return Res;
}

double & tVector :: operator[](int i)
{
   if (i >= this -> n)
	  throw "tVector: out of range.";
   return v[i];
}

double tVector :: elem(int i) const
{
   if (i >= this -> n)
	  throw "tVector: out of range.";
   return v[i];
}

tVector :: ~tVector()
{
   if (v)
	  delete[] v;
   this -> n = 0;
   v = 0;
}

tVector :: tVector(const tVector &Vec)
{
   this -> n = Vec.n;
   this -> v = new double[n];
   for (int i = 0; i < n; i++)
	  (*this)[i] = Vec.elem(i);
}

tVector tVector :: operator=(const tVector &Vec)
{
   if ( this -> n < Vec.n )
   {
	  delete[] v;
	  v = new double[Vec.n];
	  this -> n = Vec.n;
   }
   for (int i = 0; i < Vec.n; i++)
	  (*this)[i] = Vec.elem(i);
   for (int i = Vec.n; i < n; i++)
      (*this)[i] = 0; 
   return *this;
}


tVector :: tVector(const char *filename)
{
   ifstream file(filename);
   file >> n;
   this -> n = n;
   this -> v = new double[n];
   for (int i = 0; i < n; i++)
      file >> (*this)[i];
}

void tVector :: print(const char *hint, const char *filename) const
{
   int dig;
   int max_len = 0;

   for (int i = 0; i < size(); i++)
	  if ( ( dig = len( (*this).elem(i) ) ) > max_len  )
	     max_len = dig;

   ofstream file(filename);
   double num;

   if (max_len > MAX_WDTH)
	  max_len = MAX_WDTH;

   file << hint << endl;
   file << '(';
   for (int i = 0; i < size(); i++)
   {
	  num = (*this).elem(i);
	  /*if ( abs(num) < EPS )
		 num = 0; */
	  file << setw(max_len + 3) << setprecision(PREC) << num << ' ';
	  if (i != size() - 1)
	     file << ", ";
   }
   file << ')';
   file << endl;
}


tVector operator-(const tVector &Vec1, const tVector &Vec2)
{
   if ( Vec1.size() != Vec2.size() )
	  throw "Can't substract two vectors with different dimensions.";
   tVector Vec(Vec1);
   for (int i = 0; i < Vec1.size(); i++)
	  Vec[i] -= Vec2.elem(i);
   return Vec;
}

tVector operator+(const tVector &Vec1, const tVector &Vec2)
{
   if ( Vec1.size() != Vec2.size() )
	  throw "Can't add two vectors with different dimensions.";
   tVector Vec(Vec1);
   for (int i = 0; i < Vec1.size(); i++)
	  Vec[i] += Vec2.elem(i);
   return Vec;
}

double operator*(const tVector &Vec1, const tVector &Vec2)
{
   if ( Vec1.size() != Vec2.size() )
	  throw "Can't multiply two vectors with different dimensions.";
   double res = 0;
   for (int i = 0; i < Vec1.size(); i++)
	  res += Vec1.elem(i) * Vec2.elem(i);
   return res;
}

double tVector :: norm(void) const
{
   double res = 0;
   for (int i = 0; i < this -> size(); i++)
	  res += sqr(this -> elem(i));
   return sqrt(res);
}

ostream &operator<<(ostream &file, tVector Vec)
{
   int dig;
   int max_len = 0;

   for (int i = 0; i < Vec.size(); i++)
	  if ( ( dig = len( Vec.elem(i) ) ) > max_len  )
		 max_len = dig;

   double num;

   if (max_len > MAX_WDTH)
	  max_len = MAX_WDTH;

   file << '(';
   for (int i = 0; i < Vec.size(); i++)
   {
	  num = Vec.elem(i);
	  /*if ( abs(num) < EPS )
		 num = 0; */
	  file << setw(max_len + 3) << setprecision(PREC) << num << ' ';
	  if (i != Vec.size() - 1)
	     file << ", ";
   }
   file << ')';
   file << endl;
   return file;
}


#include "tMatrix.h"
#include <iostream>
#include <fstream>
#include "other.h"

using namespace std;

void print_plan(ofstream &file, int var[], int var_cnt, int n, int m, tMatrix Sim);
tMatrix init(int &n, int &m, int &var_cnt, int var[]);
int neg_free_term(tMatrix Sim, int n);
int goal_delim(tMatrix Sim, int n, int m);
int compat(tMatrix Sim, int n, int m);
void next_basic(tMatrix &Sim, int key_row, int key_col, int n, int m, int var[]);


void simplex(int var[], int &var_cnt, int &n, int &m, tMatrix &Sim)
{
	  Sim = init(n, m, var_cnt, var);

	  ofstream cout("log.txt");
	  while (1)
	  {
		 print_plan(cout, var, var_cnt, n, m, Sim);
		 cout << Sim << endl << endl << endl;

		 if (! compat(Sim, n, m))                     // система обмежень несумісна
			throw "Constrain set is incompatible.";

		 if (! goal_delim(Sim, n, m))                 // цільова функція необмежена
			throw "Goal function is unlimited.";


		 if (neg_free_term(Sim, n) == -1)  // допустимий план
		 {
			int key_row = -1; int key_col = -1;
			int min_col = n + 1;
			for (int j = n + 2; j <= m; j++)
				if (Sim.at(n + 1, j) < Sim.at(n + 1, min_col))
					min_col = j;
			key_col = min_col;

			if (Sim.at(n + 1, min_col) > 0 )  // Оптимальний і допустимий план
			{
				cout << "Optimal plan:" << endl;
				print_plan(cout, var, var_cnt, n, m, Sim);
				return;
			}
			double min_row = -1;
			for (int i = 1; i <= n; i++)
			if ( gtz(Sim.at(i, key_col)) )
			{
				if (min_row == -1)
					min_row = i;
				if ( Sim.at(i, 1) / Sim.at(i, key_col)  < Sim.at(min_row, 1) / Sim.at(min_row, key_col) )
					min_row = i;
			}
			key_row = min_row;
			if (min_row == -1) ;// Необмежена цільова функція
			next_basic(Sim, key_row, key_col, n, m, var);
		 }
		 else // План недопустимий
		 {
			int line = neg_free_term(Sim, n);
			int max_col = n + 1;
			for (int j = n + 2; j <= m; j++)
			   if ( Sim.at(line, j) > Sim.at(line, max_col) )
				  max_col = j;
			int key_col = max_col;
			if (! gtz(Sim.at(line, max_col)) )
               throw "Constrain set is incompatible.";

			int min_row = 1;
			for (int i = 2; i <= n; i++)
			{
				if ( abs(Sim.at(i, 1) / Sim.at(i, key_col))  < abs(Sim.at(min_row, 1) / Sim.at(min_row, key_col)) )
					min_row = i;
			}
			int key_row = min_row;
			next_basic(Sim, key_row, key_col, n, m, var);
		 }
	  }
}

int main()
{
   try
   {
	  int var_cnt = 0;
	  int var[MAX];
	  tMatrix Sim(0, 0);
	  int n, m;
	  simplex(var, var_cnt, n, m, Sim);
   }
   catch (const char *s)
   {
	   cout << s;
	   cin.get();
   }
}

void next_basic(tMatrix &Sim, int key_row, int key_col, int n, int m, int var[])
{
   double solving = Sim.at(key_row, key_col);
   for (int i = 1; i <= n + 1; i++)
	  for (int j = 1; j <= m; j++)
		 if ( (i != key_row) && (j != key_col) )
		 {
			Sim(i, j) -= Sim.at(key_row, j) * Sim(i, key_col) / solving;
			if ( (j != 1) && !( j >= n + 1 && j <= m) )
			   Sim(i, j) = 0;
		 }

	  Sim(key_row, key_col) = 1 / Sim.at(key_row, key_col);

	  Sim(key_row, 1) /= solving;
	  for (int j = n + 1; j <= m; j++)
	  if (j != key_col)
		 Sim(key_row, j) /= solving;
	  for (int i = 1; i <= n + 1; i++)
	  if (i != key_row)
	  {
		 Sim(i, key_col) /= solving;
		 Sim(i, key_col) *= (-1);
	  }
	  swap_var(var, key_row, key_col);
}

int compat(tMatrix Sim, int n, int m)
{
   int good = 1;
   for (int i = 1; i <= n; i++)
   if ( ltz(Sim.at(i, 1) ) )
	  {
		 int has_neg = 0;
		 for (int j = n + 1; j <= m; j++)
		 if ( ltz(Sim.at(i, j)) )
		 {
			has_neg = 1;
			break;
		 }
		 if (! has_neg)
		 {
			good = 0;
			break;
		 }
	  }
   return good;
}

int goal_delim(tMatrix Sim, int n, int m)
{
   int good = 1;
   for (int j = n + 1; j <= m; j++)
   if ( ltz(Sim.at(n + 1, j)) )
   {
	  int has_pos = 0;
	  for (int i = 1; i <= n; i++)
	  if ( gtz( Sim.at(i, j) ) )
	  {
		 has_pos = 1;
		 break;
	  }
	  if (! has_pos)
	  {
		 good = 0;
		 break;
	  }
   }
   return good;
}

int neg_free_term(tMatrix Sim, int n)
{
   for (int i = 1; i <= n; i++)
	  if ( ltz(Sim.at(i, 1)) )
		 return i;
   return -1;
}

tMatrix init(int &n, int &m, int &var_cnt, int var[])
{
   tMatrix M("input.txt");
   get_nm(&n, &m, "input.txt");
   var_cnt = m - 1;
   for (int i = 1; i <= var_cnt; i++)
	  var[i] = i;
   tVector ZZ("func.txt");
   tVector Z(m);
   Z = -1 * ZZ;

   M.append_row(Z);
   //cout << M;
   m = m - 1;
   if (M.HaussStraight(var, var_cnt) == -1)
      throw "Matrix rank is unappropriate.";
   //cout << M;
   M.canon();


   tMatrix Sim(n + 2, m + 2);    // 0 - фіктивні, середні теж
   for (int i = 1; i <= n + 1; i++)
   {
	  Sim(i, 1) = M.at(i - 1, M.col_cnt() - 1);
	  for (int j = n + 1; j <= m; j++)
		 Sim(i, j) = M.at(i - 1, j - 1);
   }
   return Sim;
}

void print_plan(ofstream &file, int var[], int var_cnt, int n, int m, tMatrix Sim)
{
	for (int i = 1; i <= n; i++)
	   file << "x" << var[i] << " = " << Sim.at(i, 1) << "   ";
	for (int i = n + 1; i <= m; i++)
	   file << "x" << var[i] << " = " << 0 << "   ";
	file << endl;
	file << "F=" << Sim.at(n + 1, 1) << endl;
}

#include "other.h"
#include <fstream>

extern int var[];
extern int var_cnt;

void get_nm(int *pn, int *pm, const char *filename)
{
   ifstream file(filename);
   file >> *pn;
   file >> *pm;
   file.close();
}

void swap_var(int var[], int i, int j)
{
   int t = var[i];
   var[i] = var[j];
   var[j] = t;
}

int gtz(double a)
{
   return (a > EPS);
}

int ltz(double a)
{
   return (a < -EPS);
}


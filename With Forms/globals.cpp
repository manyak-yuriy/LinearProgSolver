#include "globals.h"
#include <math>

int len(double a)
{
   int res = 0;
   if (a < 0)
	  a = -a;
   while (a > 1)
   {
	  res++;
	  a /= 10;
   }
   return res + PREC;
}

int is_zero(double a)
{
   double ab = fabs(a);
   if ( ab < EPS )
	  return 1;
   return 0;
}

int sign(double a)
{
   if ( abs(a) < EPS )
      return 0;
   if ( a > 0 )
	  return 1;
   return -1;
}

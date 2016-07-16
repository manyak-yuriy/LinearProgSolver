#ifndef GLOBALS_H
#define GLOBALS_H

const double EPS = 1e-13;
#define PREC 5
#define MAX_WDTH 6
#define out "out.txt"
#define sqr(a) ( (a) * (a) )


int len(double a);
int is_zero(double a);
template <class T>
inline int min(T a, T b);
int sign(double a);

#endif


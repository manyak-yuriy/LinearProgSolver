#ifndef OTHER_H
#define OTHER_H

const double EPS = 1e-10;
const int MAX = 100;

void get_nm(int *pn, int *pm, const char *filename);
int ltz(double a);
int gtz(double a);
void swap_var(int var[], int i, int j);

enum error
{
	UNLIM,
	INCOMP,
	ITER
};

#endif

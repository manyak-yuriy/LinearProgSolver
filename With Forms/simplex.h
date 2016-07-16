#ifndef SIMPLEX_H
#define SIMPLEX_H

void print_plan(ofstream &file, int var[], int var_cnt, int n, int m, tMatrix Sim);
tMatrix init(int &n, int &m, int &var_cnt, int var[]);
int neg_free_term(tMatrix Sim, int n);
int goal_delim(tMatrix Sim, int n, int m);
int compat(tMatrix Sim, int n, int m);
void next_basic(tMatrix &Sim, int key_row, int key_col, int n, int m, int var[]);
void next_basic_m(tMatrix &Sim, int key_row, int key_col, int n, int m, int var[]);
void simplex(int var[], int &var_cnt, int &n, int &m, tMatrix &Sim);

#endif

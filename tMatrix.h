#ifndef MATRIX_H
#define MATRIX_H

#include <string>
#include "tVector.h"
#include <iomanip.h>

using namespace std;

class tMatrExc
{
private:
   string *Msg_p;
public:
   tMatrExc(const char *msg);
   const char *message() const;
   ~tMatrExc();
};

class tMatrix
{
private:
   double *arr;
   int _col_cnt, _row_cnt;
public:
   tMatrix(int row_cnt, int col_cnt);
   tMatrix(const tMatrix &Matr);
   tMatrix(const char *filename);
   tMatrix(const tMatrix &Matr, tVector &Vec);

   tMatrix SubMatr(int row_1, int col_1, int row_2, int col_2) const;

   tMatrix &operator=(const tMatrix &M);
   inline double &operator()(int row, int col);

   inline double at(int row, int col) const;
   inline int col_cnt() const {return this -> _col_cnt;}
   inline int row_cnt() const {return this -> _row_cnt;}
   int is_symm() const;

   void row_add(int first, int second, double koef);
   void col_add(int first, int second, double koef);
   void row_mul(int row, double k);
   void col_mul(int col, double k);
   void row_swap(int first, int second);
   void col_swap(int first, int second);
   void append_row(const tVector Vec);

   void trans();
   double Invert(void);   // returns det A

   int isSquare(void) const;
   int diag_pref(void) const;

   int HaussStraight(int var[], int var_cnt);
   void HaussBack(void);
   void Turn_U(void);
   void Turn_L(void);

   tVector get_col(int col) const;
   tVector get_row(int row) const;

   double minor(int i_start, int i_fin, int j_start, int j_fin);

   void print(const char *hint, const char *filename) const;

   friend tVector operator*(const tMatrix &Mat1, const tVector &Vec);
   friend tMatrix operator*(const tMatrix &Mat1, const tMatrix &Mat2);
   friend ostream &operator<<(ostream &file, tMatrix M);

   void Get_DLMatr(tMatrix &D, tMatrix &L);

   void canon(void);

   ~tMatrix();
};

#endif MATRIX_H
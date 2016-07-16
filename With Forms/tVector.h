#ifndef VECTOR_H
#define VECTOR_H
#include <fstream>

class tVector
{
private:
   double *v;
   int n;
public:
   tVector(int n);
   tVector(const tVector &Vec);
   tVector(const char *filename);
   tVector operator=(const tVector &Vec);
   inline double elem(int i) const;
   double norm(void) const;

   inline int size(void) const {return n;}
   void print(const char *hint, const char *filename) const;

   inline double &operator[](int i);
   friend tVector operator-(const tVector &Vec1, const tVector &Vec2);
   friend tVector operator+(const tVector &Vec1, const tVector &Vec2);
   friend double operator*(const tVector &Vec1, const tVector &Vec2);
   friend tVector operator*(double k, const tVector &Vec);
   friend ostream &operator<<(ostream &file, tVector Vec);

   ~tVector();
};

#endif

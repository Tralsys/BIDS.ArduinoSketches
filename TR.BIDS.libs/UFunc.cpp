//UFunc.cpp
//Under the CC0 License
//written by Tetsu Otter

#include <stdlib.h>
#include "UFunc.h"

void LD2CA(char* ca, int startIndex, int inlen, int dnlen, double ld)
{
  int iv = (int)ld;
  double dv = ld - (double)iv;
  int ivd = 1;
  if (iv == 0) {
    int i = 0;
    for (i = 0; i < inlen; i++)
      ca[startIndex + i] = ' ';
    ca[startIndex + inlen - 1] = '0';
  } else {
    for (int i = 0; i < inlen; i++) {
      ca[startIndex + inlen - 1 - i] = iv == 0 ? ' ' : ((char)(iv % 10) + '0');
      iv /= 10;
    }
  }
  if (dnlen > 0) {
    ca[startIndex + inlen] = '.';
    for (int i = 0; i < dnlen; i++) {
      dv *= 10;
      ca[startIndex + inlen + 1 + i] = (char)((int)dv % 10) + '0';
    }
  }
}
void ZeroFill(char *c, int l)
{
  for (int i = 0; i < l; i++)
    c[i] = 0;
}

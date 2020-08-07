#ifndef _VEC_TYPE_H_
#define _VEC_TYPE_H_

#include <stdint.h>

#define DECL_VEC(type, len)  typedef type type##len __attribute__((ext_vector_type(len)))

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

DECL_VEC(char, 4);
DECL_VEC(char, 8);
DECL_VEC(char, 16);
DECL_VEC(uchar, 4);
DECL_VEC(uchar, 8);
DECL_VEC(uchar, 16);
DECL_VEC(short, 4);
DECL_VEC(short, 8);
DECL_VEC(short, 16);
DECL_VEC(ushort, 4);
DECL_VEC(ushort, 8);
DECL_VEC(ushort, 16);
DECL_VEC(int, 4);
DECL_VEC(int, 8);
DECL_VEC(int, 16);
DECL_VEC(uint, 4);
DECL_VEC(uint, 8);
DECL_VEC(uint, 16);

#endif

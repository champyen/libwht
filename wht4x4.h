#ifndef _WHT4X4_H_
#define _WHT4X4_H_

#include "vec_type.h"

void dual_wht4x4(uchar *src, int src_pitch, short *dst, int dst_pitch);
void dual_iwht4x4(short *src, int src_pitch, uchar *dst, int dst_pitch);
void quad_wht4x4(uchar *src, int src_pitch, short *dst, int dst_pitch);
void quad_iwht4x4(short *src, int src_pitch, uchar *dst, int dst_pitch);

void dump_matrix4x4_u8(uchar *mat, int pitch);
void dump_matrix4x4_s16(short *mat, int pitch);

#endif
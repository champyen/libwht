#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "vec_type.h"

void __wht4x4(short8 *data)
{
    short8 tmp[2];

    tmp[0].lo = data[0].lo + data[0].hi + data[1].lo + data[1].hi;
    tmp[0].hi = data[0].lo + data[0].hi - data[1].lo - data[1].hi;
    tmp[1].lo = data[0].lo - data[0].hi - data[1].lo + data[1].hi;
    tmp[1].hi = data[0].lo - data[0].hi + data[1].lo - data[1].hi;

    /* 0, 2 */
    //data[0] = __builtin_shufflevector( tmp[0], tmp[1], 0, 2, 8, 10, 4, 6, 12, 14 );
    data[0] = __builtin_shufflevector( tmp[0], tmp[1], 0, 2, 4, 6, 8, 10, 12, 14 );
    /* 1, 3 */
    //data[1] = __builtin_shufflevector( tmp[0], tmp[1], 1, 3, 9, 11, 5, 7, 13, 15 );
    data[1] = __builtin_shufflevector( tmp[0], tmp[1], 1, 3, 5, 7, 9, 11, 13, 15 );
    //tmp[0] = __builtin_shufflevector( data[0], data[1], 0, 4, 2, 6, 8, 12, 10, 14 );
    tmp[0] = __builtin_shufflevector( data[0], data[1], 0, 2, 4, 6, 8, 10, 12, 14 );
    tmp[1] = __builtin_shufflevector( data[0], data[1], 1, 3, 5, 7, 9, 11, 13, 15 );

    data[0].lo = (tmp[0].lo + tmp[0].hi + tmp[1].lo + tmp[1].hi);
    data[0].hi = (tmp[0].lo + tmp[0].hi - tmp[1].lo - tmp[1].hi);
    data[1].lo = (tmp[0].lo - tmp[0].hi - tmp[1].lo + tmp[1].hi);
    data[1].hi = (tmp[0].lo - tmp[0].hi + tmp[1].lo - tmp[1].hi);

    data[0] >>= 2;
    data[1] >>= 2;

    /* 0, 2 */
    tmp[0] = __builtin_shufflevector( data[0], data[1], 0, 2, 4, 6, 8, 10, 12, 14 );
    /* 1, 3 */
    tmp[1] = __builtin_shufflevector( data[0], data[1], 1, 3, 5, 7, 9, 11, 13, 15 );
    data[0] = __builtin_shufflevector( tmp[0], tmp[1], 0, 2, 4, 6, 8, 10, 12, 14 );
    data[1] = __builtin_shufflevector( tmp[0], tmp[1], 1, 3, 5, 7, 9, 11, 13, 15 );
}


void wht4x4(uchar *src, int src_pitch, short *dst, int dst_pitch)
{
    short8 data[2];
    for(int i = 0; i < 2; i++){
        data[i].lo = __builtin_convertvector( *((uchar4*)(src+(i*2)*src_pitch)), short4);
        data[i].hi = __builtin_convertvector( *((uchar4*)(src+(i*2+1)*src_pitch)), short4);
    }

    __wht4x4(data);

    for(int i = 0; i < 2; i++){
        *((short4*)(dst+(i*2)*dst_pitch)) = data[i].lo;
        *((short4*)(dst+(i*2+1)*dst_pitch)) = data[i].hi;
    }
}

void iwht4x4(short *src, int src_pitch, uchar *dst, int dst_pitch)
{
    short8 data[2];
    for(int i = 0; i < 2; i++){
        data[i].lo = *((short4*)(src+(i*2)*src_pitch));
        data[i].hi = *((short4*)(src+(i*2+1)*src_pitch));
    }

    __wht4x4(data);

    for(int i = 0; i < 2; i++){
        *((uchar4*)(dst+(i*2)*dst_pitch)) = __builtin_convertvector(data[i].lo, uchar4);
        *((uchar4*)(dst+(i*2+1)*dst_pitch)) = __builtin_convertvector(data[i].hi, uchar4);
    }
}

void __dual_wht4x4(short8 *data)
{
    short8 tmp[4];

    tmp[0] = data[0] + data[1] + data[2] + data[3];
    tmp[1] = data[0] + data[1] - data[2] - data[3];
    tmp[2] = data[0] - data[1] - data[2] + data[3];
    tmp[3] = data[0] - data[1] + data[2] - data[3];

    /* Transpose */
    data[0] = __builtin_shufflevector( tmp[0], tmp[2], 0, 2, 8, 10, 4, 6, 12, 14 );
    data[1] = __builtin_shufflevector( tmp[0], tmp[2], 1, 3, 9, 11, 5, 7, 13, 15 );
    data[2] = __builtin_shufflevector( tmp[1], tmp[3], 0, 2, 8, 10, 4, 6, 12, 14 );
    data[3] = __builtin_shufflevector( tmp[1], tmp[3], 1, 3, 9, 11, 5, 7, 13, 15 );
    tmp[0] = __builtin_shufflevector( data[0], data[2], 0, 8, 2, 10, 4, 12, 6, 14 );
    tmp[2] = __builtin_shufflevector( data[0], data[2], 1, 9, 3, 11, 5, 13, 7, 15 );
    tmp[1] = __builtin_shufflevector( data[1], data[3], 0, 8, 2, 10, 4, 12, 6, 14 );
    tmp[3] = __builtin_shufflevector( data[1], data[3], 1, 9, 3, 11, 5, 13, 7, 15 );

    data[0] = (tmp[0] + tmp[1] + tmp[2] + tmp[3]) >> 2;
    data[1] = (tmp[0] + tmp[1] - tmp[2] - tmp[3]) >> 2;
    data[2] = (tmp[0] - tmp[1] - tmp[2] + tmp[3]) >> 2;
    data[3] = (tmp[0] - tmp[1] + tmp[2] - tmp[3]) >> 2;

    /* Transpose */
    tmp[0] = __builtin_shufflevector( data[0], data[2], 0, 2, 8, 10, 4, 6, 12, 14 );
    tmp[1] = __builtin_shufflevector( data[0], data[2], 1, 3, 9, 11, 5, 7, 13, 15 );
    tmp[2] = __builtin_shufflevector( data[1], data[3], 0, 2, 8, 10, 4, 6, 12, 14 );
    tmp[3] = __builtin_shufflevector( data[1], data[3], 1, 3, 9, 11, 5, 7, 13, 15 );
    data[0] = __builtin_shufflevector( tmp[0], tmp[2], 0, 8, 2, 10, 4, 12, 6, 14 );
    data[2] = __builtin_shufflevector( tmp[0], tmp[2], 1, 9, 3, 11, 5, 13, 7, 15 );
    data[1] = __builtin_shufflevector( tmp[1], tmp[3], 0, 8, 2, 10, 4, 12, 6, 14 );
    data[3] = __builtin_shufflevector( tmp[1], tmp[3], 1, 9, 3, 11, 5, 13, 7, 15 );

}

void dual_wht4x4(uchar *src, int src_pitch, short *dst, int dst_pitch)
{
    short8 data[4];
    for(int i = 0; i < 4; i++)
        data[i] = __builtin_convertvector( *((uchar8*)(src+i*src_pitch)), short8);

    __dual_wht4x4(data);

    for(int i = 0; i < 4; i++)
        *((short8*)(dst+i*dst_pitch)) = data[i];
}

void dual_iwht4x4(short *src, int src_pitch, uchar *dst, int dst_pitch)
{
    short8 data[4];
    for(int i = 0; i < 4; i++)
        data[i] = *((short8*)(src+i*src_pitch));

    __dual_wht4x4(data);

    for(int i = 0; i < 4; i++)
        *((uchar8*)(dst+i*dst_pitch)) = __builtin_convertvector(data[i], uchar8);
}

void __quad_wht4x4(short16 *data)
{
    short16 tmp[4];

    tmp[0] = data[0] + data[1] + data[2] + data[3];
    tmp[1] = data[0] + data[1] - data[2] - data[3];
    tmp[2] = data[0] - data[1] - data[2] + data[3];
    tmp[3] = data[0] - data[1] + data[2] - data[3];

    /* Transpose */
    data[0] = __builtin_shufflevector( tmp[0], tmp[2], 0, 2, 16, 18, 4, 6, 20, 22, 8, 10, 24, 26, 12, 14, 28, 30 );
    data[1] = __builtin_shufflevector( tmp[0], tmp[2], 1, 3, 17, 19, 5, 7, 21, 23, 9, 11, 25, 27, 13, 15, 29, 31 );
    data[2] = __builtin_shufflevector( tmp[1], tmp[3], 0, 2, 16, 18, 4, 6, 20, 22, 8, 10, 24, 26, 12, 14, 28, 30 );
    data[3] = __builtin_shufflevector( tmp[1], tmp[3], 1, 3, 17, 19, 5, 7, 21, 23, 9, 11, 25, 27, 13, 15, 29, 31 );
    tmp[0] = __builtin_shufflevector( data[0], data[2], 0, 16, 2, 18, 4, 20, 6, 22, 8, 24, 10, 26, 12, 28, 14, 30 );
    tmp[2] = __builtin_shufflevector( data[0], data[2], 1, 17, 3, 19, 5, 21, 7, 23, 9, 25, 11, 27, 13, 29, 15, 31 );
    tmp[1] = __builtin_shufflevector( data[1], data[3], 0, 16, 2, 18, 4, 20, 6, 22, 8, 24, 10, 26, 12, 28, 14, 30 );
    tmp[3] = __builtin_shufflevector( data[1], data[3], 1, 17, 3, 19, 5, 21, 7, 23, 9, 25, 11, 27, 13, 29, 15, 31 );

    data[0] = (tmp[0] + tmp[1] + tmp[2] + tmp[3]) >> 2;
    data[1] = (tmp[0] + tmp[1] - tmp[2] - tmp[3]) >> 2;
    data[2] = (tmp[0] - tmp[1] - tmp[2] + tmp[3]) >> 2;
    data[3] = (tmp[0] - tmp[1] + tmp[2] - tmp[3]) >> 2;

    /* Transpose */
    tmp[0] = __builtin_shufflevector( data[0], data[2], 0, 2, 16, 18, 4, 6, 20, 22, 8, 10, 24, 26, 12, 14, 28, 30 );
    tmp[1] = __builtin_shufflevector( data[0], data[2], 1, 3, 17, 19, 5, 7, 21, 23, 9, 11, 25, 27, 13, 15, 29, 31 );
    tmp[2] = __builtin_shufflevector( data[1], data[3], 0, 2, 16, 18, 4, 6, 20, 22, 8, 10, 24, 26, 12, 14, 28, 30 );
    tmp[3] = __builtin_shufflevector( data[1], data[3], 1, 3, 17, 19, 5, 7, 21, 23, 9, 11, 25, 27, 13, 15, 29, 31 );
    data[0] = __builtin_shufflevector( tmp[0], tmp[2], 0, 16, 2, 18, 4, 20, 6, 22, 8, 24, 10, 26, 12, 28, 14, 30 );
    data[2] = __builtin_shufflevector( tmp[0], tmp[2], 1, 17, 3, 19, 5, 21, 7, 23, 9, 25, 11, 27, 13, 29, 15, 31 );
    data[1] = __builtin_shufflevector( tmp[1], tmp[3], 0, 16, 2, 18, 4, 20, 6, 22, 8, 24, 10, 26, 12, 28, 14, 30 );
    data[3] = __builtin_shufflevector( tmp[1], tmp[3], 1, 17, 3, 19, 5, 21, 7, 23, 9, 25, 11, 27, 13, 29, 15, 31 );
}

void quad_wht4x4(uchar *src, int src_pitch, short *dst, int dst_pitch)
{
    short16 data[4];
    for(int i = 0; i < 4; i++)
        data[i] = __builtin_convertvector( *((uchar16*)(src+i*src_pitch)), short16);

    __quad_wht4x4(data);

    for(int i = 0; i < 4; i++)
        *((short16*)(dst+i*dst_pitch)) = data[i];
}

void quad_iwht4x4(short *src, int src_pitch, uchar *dst, int dst_pitch)
{
    short16 data[4];
    for(int i = 0; i < 4; i++)
        data[i] = *((short16*)(src+i*src_pitch));

    __quad_wht4x4(data);

    for(int i = 0; i < 4; i++)
        *((uchar16*)(dst+i*dst_pitch)) = __builtin_convertvector(data[i], uchar16);
}


void dump_matrix4x4_u8(uchar *mat, int pitch)
{
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < pitch ; j++){
            printf("%3u ", mat[i*pitch + j]);
            if((j & 0x3) == 0x3)
                printf("  ");
        }
        printf("\n");
    }
    printf("\n");
}

void dump_matrix4x4_s16(short *mat, int pitch)
{
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < pitch ; j++){
            printf("%4d ", mat[i*pitch + j]);
            if((j & 0x3) == 0x3)
                printf("     ");
        }
        printf("\n");
    }
    printf("\n");
}

#ifdef WHT_TEST
int main(void)
{
    uchar input[64]  __attribute__((aligned(32)));
    short output[64]  __attribute__((aligned(32)));
    srand(time(NULL));

    for(int i = 0; i < 16; i++){
        input[i] = rand() & 0xFF;
    }
    printf("Input:\n");
    dump_matrix4x4_u8(input, 4);

    wht4x4(input, 4, output, 4);
    printf("WHT 4x4:\n");
    dump_matrix4x4_s16(output, 4);

    iwht4x4(output, 4, input, 4);
    printf("iWHT 4x4:\n");
    dump_matrix4x4_u8(input, 4);


    for(int i = 0; i < 32; i++){
        input[i] = rand() & 0xFF;
    }
    printf("Dual Input:\n");
    dump_matrix4x4_u8(input, 8);

    dual_wht4x4(input, 8, output, 8);
    printf("2WHT 4x4:\n");
    dump_matrix4x4_s16(output, 8);

    dual_iwht4x4(output, 8, input, 8);
    printf("i2WHT 4x4:\n");
    dump_matrix4x4_u8(input, 8);




    for(int i = 0; i < 64; i++){
        input[i] = rand() & 0xFF;
    }
    printf("Quad Input:\n");
    dump_matrix4x4_u8(input, 16);

    quad_wht4x4(input, 16, output, 16);
    printf("QWHT 4x4:\n");
    dump_matrix4x4_s16(output, 16);

    quad_iwht4x4(output, 16, input, 16);
    printf("iQWHT 4x4:\n");
    dump_matrix4x4_u8(input, 16);

}
#endif

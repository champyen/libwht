#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "vec_type.h"

void __wht8x8(short8 *data)
{
    short8 tmp[8];
    tmp[0] = data[0] + data[1] + data[2] + data[3] + data[4] + data[5] + data[6] + data[7];
    tmp[1] = data[0] + data[1] + data[2] + data[3] - data[4] - data[5] - data[6] - data[7];
    tmp[2] = data[0] + data[1] - data[2] - data[3] - data[4] - data[5] + data[6] + data[7];
    tmp[3] = data[0] + data[1] - data[2] - data[3] + data[4] + data[5] - data[6] - data[7];
    tmp[4] = data[0] - data[1] - data[2] + data[3] + data[4] - data[5] - data[6] + data[7];
    tmp[5] = data[0] - data[1] - data[2] + data[3] - data[4] + data[5] + data[6] - data[7];
    tmp[6] = data[0] - data[1] + data[2] - data[3] - data[4] + data[5] - data[6] + data[7];
    tmp[7] = data[0] - data[1] + data[2] - data[3] + data[4] - data[5] + data[6] - data[7];

    for(int i = 0; i < 8; i++){
        data[0][i] = tmp[i][0];
        data[1][i] = tmp[i][1];
        data[2][i] = tmp[i][2];
        data[3][i] = tmp[i][3];
        data[4][i] = tmp[i][4];
        data[5][i] = tmp[i][5];
        data[6][i] = tmp[i][6];
        data[7][i] = tmp[i][7];
    }

    tmp[0] = (data[0] + data[1] + data[2] + data[3] + data[4] + data[5] + data[6] + data[7] + 4)>>3;
    tmp[1] = (data[0] + data[1] + data[2] + data[3] - data[4] - data[5] - data[6] - data[7] + 4)>>3;
    tmp[2] = (data[0] + data[1] - data[2] - data[3] - data[4] - data[5] + data[6] + data[7] + 4)>>3;
    tmp[3] = (data[0] + data[1] - data[2] - data[3] + data[4] + data[5] - data[6] - data[7] + 4)>>3;
    tmp[4] = (data[0] - data[1] - data[2] + data[3] + data[4] - data[5] - data[6] + data[7] + 4)>>3;
    tmp[5] = (data[0] - data[1] - data[2] + data[3] - data[4] + data[5] + data[6] - data[7] + 4)>>3;
    tmp[6] = (data[0] - data[1] + data[2] - data[3] - data[4] + data[5] - data[6] + data[7] + 4)>>3;
    tmp[7] = (data[0] - data[1] + data[2] - data[3] + data[4] - data[5] + data[6] - data[7] + 4)>>3;

    for(int i = 0; i < 8; i++){
        data[0][i] = tmp[i][0];
        data[1][i] = tmp[i][1];
        data[2][i] = tmp[i][2];
        data[3][i] = tmp[i][3];
        data[4][i] = tmp[i][4];
        data[5][i] = tmp[i][5];
        data[6][i] = tmp[i][6];
        data[7][i] = tmp[i][7];
    }
}


void dump_matrix8x8_s16(short *mat, int pitch)
{
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < pitch ; j++){
            printf("%4d ", mat[i*pitch + j]);
            if((j & 0x7) == 0x7)
                printf("     ");
        }
        printf("\n");
    }
    printf("\n");
}

#ifdef WHT_TEST
int main(void)
{
    short input[64]  __attribute__((aligned(32)));
    short output[64]  __attribute__((aligned(32)));
    srand(time(NULL));

    for(int i = 0; i < 64; i++){
        input[i] = rand() & 0xFF;
    }
    printf("Input:\n");
    dump_matrix8x8_s16(input, 8);

    __wht8x8((short8*)input);
    printf("WHT 4x4:\n");
    dump_matrix8x8_s16(input, 8);

    __wht8x8((short8*)input);
    printf("iWHT 4x4:\n");
    dump_matrix8x8_s16(input, 8);

    return 0;
}
#endif
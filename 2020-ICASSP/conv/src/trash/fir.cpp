#include <iostream>
#include <stdio.h>
#include "fir.h"
using namespace std;

static systolic<data_t, coef_t, sum_t> s[N_COEFF];

uint16_t barrett_reduce(uint16_t a)
{
#pragma HLS INLINE
  uint32_t u;

  u = ((uint32_t) a * 5) >> 16;
  u *= 12289;
  a -= u;
  return a;
}

uint16_t montgomery_reduce(uint32_t a)
{

static const uint32_t qinv = 12287; // -inverse_mod(p,2^18)
static const uint32_t rlog = 18;

#pragma HLS INLINE
  uint32_t u;

  u = (a * qinv);
  u &= ((1<<rlog)-1);
  u *= 12289;
  a = a + u;
  return a >> 18;
}

void PE(data_t x, coef_t h[N_COEFF], sum_t *y)
{
#pragma HLS INTERFACE axis off port=x
#pragma HLS INTERFACE axis off port=y


#pragma HLS ARRAY_PARTITION variable=h complete dim=1
#pragma HLS ARRAY_PARTITION variable=s complete dim=1

#pragma HLS PIPELINE


  data_t x_int[N_COEFF];
  sum_t y_int[N_COEFF];
  sum_t tmp = 0;

connect:
  for (int i = 0; i < N_COEFF; i++)
  {
      #pragma HLS unroll
    // printf("i:%d x -- in mont:%d\n",i, x);
    if (i == 0)
      s[0].exec(x, h[i], tmp, x_int[i], y_int[i]);
    else
      s[i].exec(x_int[i - 1], h[i], y_int[i - 1], x_int[i], y_int[i]);

    *y = y_int[N_COEFF-1];
  }
}

//TODO: check the types!!! some are unsigend and some are signed!!
void fir(data_t X[SIZE_SIGNAL], coef_t H[N_COEFF], data_t out[N_COEFF] ){
    sum_t Y = 0;
    DATA_T tmp_out_reg[N_COEFF];
    int16_t mont_coef [N_COEFF];
    DATA_T mont_x;
    int start = N_COEFF+2;
    int j=0; //index of the output

    init_loop: for (int i = 0; i < N_COEFF; i++) {
       #pragma HLS PIPELINE II=1
        mont_coef[i] = montgomery_reduce(3186*H[i]);//coeff_reg is no in montgomery domain
    }

    for (int i = 0; i < SIZE_SIGNAL; i++){
      #pragma HLS PIPELINE II=1
      // mont_x = montgomery_reduce(3186 * X[i]);
      // PE(mont_x, H, &Y);
      PE(X[i], mont_coef, &Y);

      // out[i]=Y;
      // printf("Y:%d ---",Y);
      if (i >= start ){ //start to generate outputs
        if ( i >= start + N_COEFF){
            out[j] = (tmp_out_reg[i-start-N_COEFF] - Y); //not sure if this line is true if we want to do it in mod q
            // printf("i:%d -- j:%d +++ out[j]:%d -- tmp_out_reg[i-start]:%d \n",i,j,out[j], tmp_out_reg[i-start] );
            j++;
            }
        else    {
            tmp_out_reg[i-start] = (Y);
            // printf("i:%d -- j:%d +++ out[j]:%d -- tmp_out_reg[i-start]:%d \n",i,j,out[j], tmp_out_reg[i-start] );
        }
      }
      
    }
}

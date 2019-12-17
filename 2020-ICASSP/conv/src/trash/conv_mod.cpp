#include "conv.h"

#define N_COEFF 512


uint16_t modq(uint32_t a){ //taken from https://eprint.iacr.org/2014/514.pdf
#pragma HLS INLINE
  uint32_t u;
  u = a - 12289 * ((a >> 14) + (a >> 16) + (a >> 18) + (a >> 20) + (a >> 22) + (a >> 24));
  
  for (int i=0; i < 9; i++)
  {
       u = u - PARAM_Q;
    //   if (u>=PARAM_Q)
    //   {
    //     u = u - PARAM_Q;
    //   }
    //   else
    //   {
    //       break;
    //   }
      
  }
//   while(u>=PARAM_Q)
//     #pragma HLS latency min=0 max=9
//     u = u - PARAM_Q;

  return u;
}

uint16_t barrett_reduce(uint16_t a)
{
#pragma HLS INLINE
  uint32_t u;

  u = ((uint32_t) a * 5) >> 16;
  u *= PARAM_Q;
  a -= u;
  return a;
}


// void conv (DATA_T *output_r, DATA_T *signal_r, DATA_T *coeff) {
// #pragma HLS INTERFACE m_axi  depth=1024 port = output_r offset = slave bundle = gmem
// #pragma HLS INTERFACE m_axi  depth=1024 port = signal_r offset = slave bundle = gmem
// #pragma HLS INTERFACE m_axi  depth=1024 port = coeff offset = slave bundle = gmem
// #pragma HLS INTERFACE s_axilite port = output_r bundle = control
// #pragma HLS INTERFACE s_axilite port = signal_r bundle = control
// #pragma HLS INTERFACE s_axilite port = coeff bundle = control
// #pragma HLS INTERFACE s_axilite port = return bundle = control

void conv (DATA_T output_r [N_COEFF], DATA_T signal_r [N_COEFF], DATA_T coeff [N_COEFF]) {
    
    DATA_T coeff_reg[N_COEFF];
    DATA_T shift_reg[N_COEFF];
    DATA_T tmp_out_reg[N_COEFF];
#pragma HLS ARRAY_PARTITION variable=shift_reg complete dim=0

init_loop: for (int i = 0; i < N_COEFF; i++) {
       #pragma HLS PIPELINE II=1
        shift_reg[i] = 0;
        coeff_reg[i] = coeff[i];//coeff_reg is no in montgomery domain
    }

outer_loop: for (int j = 0; j < N_COEFF*2; j++) {
       #pragma HLS PIPELINE II=1
        DATA_T acc = 0;
        uint32_t mult_tmp;
        DATA_T x;
        if ( j < N_COEFF )
            x = signal_r[j];
        else 
            x = 0;
            

    //this loop will be flattenend
    shift_loop: for (int i = N_COEFF - 1; i >= 0; i--) {
            if (i == 0) {
                mult_tmp = modq(x * coeff_reg[0]);
                shift_reg[0] = x;
            } else {
                shift_reg[i] = shift_reg[i - 1];
                mult_tmp = modq(shift_reg[i] * coeff_reg[i]); 
                
            }
            // acc = (acc + mult_tmp);
            acc = barrett_reduce(acc + mult_tmp);
    }
      
        //to the modular reduction (x^n+1) ==> ax^(n+j) = -ax^(j) ==> output_r[j]=output_r[j]-output_r[j+n]
        if ( j >= N_COEFF ){
            output_r[j-N_COEFF] = barrett_reduce(tmp_out_reg[j-N_COEFF] - acc); //not sure if this line is true if we want to do it in mod q
            }
        else    {
            tmp_out_reg[j] = (acc);
        }
            
        
    }
}


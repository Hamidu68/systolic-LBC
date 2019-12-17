#include "conv.h"

#define N_COEFF 512


uint16_t barrett_reduce(uint16_t a)
{
#pragma HLS INLINE
  uint32_t u;

  u = ((uint32_t) a * 5) >> 16;
  u *= PARAM_Q;
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
  u *= PARAM_Q;
  a = a + u;
  return a >> 18;
}



/*W is in montgomery domain*/
// void PE (DATA_T x_in, DATA_T y_in, DATA_T w, DATA_T xout[1],DATA_T y_out[1]){   
void PE (DATA_T w, DATA_T x_in, DATA_T y_in, DATA_T out[2]){   
    #pragma HLS ALLOCATION instances=mul limit=1 operation
    #pragma HLS ARRAY_PARTITION variable=out complete dim=0

    out[0] = x_in;
    DATA_T res_tmp = montgomery_reduce(x_in*w);
    out[1] = barrett_reduce(y_in + res_tmp);
}


#define N 8
void mult (DATA_T x_in[N], DATA_T W[N], DATA_T out[N*2]){  

    // static DATA_T shift_registers[ٔN];

    #pragma HLS ARRAY_PARTITION variable=W complete dim=0

    for (int i=0; i<N; i=i+1){
        #pragma HLS UNROLL

        if (I==0)
            PE(W[i], datain ),; #(dsize) fte_inst0 (clk, h[I], datain, {32{1'b0}}, arraydata[I], arrayprod[I]);
        else
            PE(W[i]);mac_element #(dsize) fte_inst  (clk, h[I], arraydata[I-1], arrayprod[I-1], arraydata[I], arrayprod[I]);
    }

    out[0] = x_in;
    DATA_T res_tmp = montgomery_reduce(x_in*w);
    out[1] = barrett_reduce(y_in + res_tmp);
}

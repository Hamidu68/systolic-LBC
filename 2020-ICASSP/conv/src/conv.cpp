#include "conv.h"

void conv (DATA_T *output_r, DATA_T *signal_r, DATA_T *coeff) {

#pragma HLS INTERFACE m_axi port = output_r offset = slave bundle = gmem
#pragma HLS INTERFACE m_axi port = signal_r offset = slave bundle = gmem
#pragma HLS INTERFACE m_axi port = coeff offset = slave bundle = gmem
#pragma HLS INTERFACE s_axilite port = output_r bundle = control
#pragma HLS INTERFACE s_axilite port = signal_r bundle = control
#pragma HLS INTERFACE s_axilite port = coeff bundle = control
#pragma HLS INTERFACE s_axilite port = return bundle = control

    int coeff_reg[N_COEFF];
    int shift_reg[N_COEFF];
   #pragma HLS ARRAY_PARTITION variable=shift_reg complete dim=0

init_loop: for (int i = 0; i < N_COEFF; i++) {
       #pragma HLS PIPELINE II=1
        shift_reg[i] = 0;
        coeff_reg[i] = coeff[i];
    }

outer_loop: for (int j = 0; j < N_COEFF*2; j++) {
       #pragma HLS PIPELINE II=1
        DATA_T acc = 0;
        DATA_T x = signal_r[j];

    //this loop will be flattenend
    shift_loop: for (int i = N_COEFF - 1; i >= 0; i--) {
            if (i == 0) {
                acc += x * coeff_reg[0];
                shift_reg[0] = x;
            } else {
                shift_reg[i] = shift_reg[i - 1];
                acc += shift_reg[i] * coeff_reg[i];
            }
        }
        output_r[j] = acc;
    }
}

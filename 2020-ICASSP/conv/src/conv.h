#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "params.h"
#include "reduce.h"


#ifndef PARAM_N
    #define N_COEFF 64
#else
    #define N_COEFF PARAM_N
#endif

#define SIGNAL_SIZE N_COEFF
#define OUT_SIZE SIGNAL_SIZE

uint16_t montgomery_reduce(uint32_t a);
uint16_t barrett_reduce(uint16_t a);


void conv(DATA_T *output_r, DATA_T *signal_r, DATA_T *coeff) ;




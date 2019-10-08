#include <string>
#include <vector>
#include "inttypes.h"
#include <stdint.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


using namespace std;
#define N_COEFF 5

#define SIGNAL_SIZE 5
#define OUT_SIZE SIGNAL_SIZE*2
typedef uint16_t DATA_T;



void conv(DATA_T *output_r, DATA_T *signal_r, DATA_T *coeff) ;




#include "conv.h"

DATA_T PE (DATA_T x_in, DATA_T coeff, DATA_T acc){ 
#pragma HLS INLINE

#ifdef MONTGOMERY
    DATA_T mult_tmp = montgomery_reduce(x_in * coeff); 
#else 
    DATA_T mult_tmp = modq(x_in * coeff); 
#endif

    mult_tmp =  barrett_reduce(acc + mult_tmp);                
    return mult_tmp;
}

DATA_T systolic(DATA_T shift_reg[N_COEFF], DATA_T coeff_reg [N_COEFF], DATA_T  x){
    #pragma HLS INLINE
    // #pragma HLS ALLOCATION instances=mul limit=64 operation

    int i, j;
    DATA_T acc = 0;
    DATA_T mult_tmp;
    // DATA_T x;
    //this loop will be flattenend
    dely_loop: for (i = N_COEFF - 1; i > 0; i--) {
            shift_reg[i] = shift_reg[i - 1];
    }

//i=0
    shift_reg[0] = x;
    mult_tmp = PE(x, coeff_reg[0], acc);
    acc = mult_tmp; 

//mac operation
    MAC: for (i = N_COEFF - 1; i > 0; i--) {
        mult_tmp = PE(shift_reg[i], coeff_reg[i], acc);
        acc = mult_tmp;     
    }
    
    
    return acc;
}


void conv (DATA_T output_r [N_COEFF], DATA_T signal_r [N_COEFF], DATA_T coeff [N_COEFF]) {


    DATA_T coeff_reg[N_COEFF];
    DATA_T shift_reg[N_COEFF];
    DATA_T tmp_out_reg[N_COEFF];
    int i, j;
    uint32_t temp, temp2;
#pragma HLS ARRAY_PARTITION variable=shift_reg complete dim=1

init_loop:
     for (i = 0; i < N_COEFF; i++) {
       #pragma HLS PIPELINE II=1
        shift_reg[i] = 0;
#ifdef MONTGOMERY
        // temp = 3186*coeff[i];
        temp = coeff[i];
        temp2 = (temp<<11)+(temp<<10)+(temp<<6)+(temp<<5)+(temp<<4)+(temp<<1);

        coeff_reg[i] = montgomery_reduce(temp2);//coeff_reg is now in montgomery domain
#else 
        coeff_reg[i] = coeff[i];//coeff_reg is in normal domain
#endif


    }

outer_loop: 
    for (j = 0; j < N_COEFF*2; j++) {
       #pragma HLS PIPELINE II=1

        DATA_T x;
        

        if ( j < N_COEFF )
            x = signal_r[j];
        else 
            x = 0;

        DATA_T acc =  systolic(shift_reg, coeff_reg, x);
        
        
        //to the modular reduction (x^n+1) ==> ax^(n+j) = -ax^(j) ==> output_r[j]=output_r[j]-output_r[j+n]
        if ( j < N_COEFF )
            tmp_out_reg[j] = (acc);
        else 
            output_r[j-N_COEFF] = barrett_reduce(tmp_out_reg[j-N_COEFF] - acc); //not sure if this line is true if we want to do it in mod q            
        
    }
}
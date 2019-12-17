#include "reduce.h"
#include "params.h"
#include <stdint.h>

/* Incomplete-reduction routines; for details on allowed input ranges
 * and produced output ranges, see the description in the paper: 
 * https://cryptojedi.org/papers/#newhope */



uint16_t barrett_reduce(uint16_t a){
#pragma HLS INLINE
  uint32_t u, u_temp, temp2;

  u = ((a<<2)+(a)) >> 16; //TODO: check if reduce is working correctly; I replaced mults with shifts
  // u = ((uint32_t) a * 5) >> 16;
  
  // u_temp = u * 12289;
  u_temp = (u<<13) + (u<<12) + u;

  a = a - u_temp;

  return a;
}

uint16_t montgomery_reduce(uint32_t a){

#pragma HLS INLINE
  uint32_t u, u_temp, u_temp2;

  // u = (a * 12287);
  u = (a<<13)+(a<<11)+(a<<10)+(a<<9)+(a<<8)+(a<<7)+(a<<6)+(a<<5)+(a<<4)+(a<<3)+(a<<2)+(a<<1)+(a);


  u_temp = u & ((1<<18)-1);


  // u_temp2 = u_temp * 12289;
  u_temp2 = (u_temp<<13) + (u_temp<<12) + u_temp;

  a = a + u_temp2;

  return a >> 18;
}



uint16_t modq(uint32_t a){ //taken from https://eprint.iacr.org/2014/514.pdf
#pragma HLS INLINE
  uint32_t u;
  u = a - 12289 * ((a >> 14) + (a >> 16) + (a >> 18) + (a >> 20) + (a >> 22) + (a >> 24));
  
  for (int i=0; i < 9; i++)
  {
      if (u>=PARAM_Q)
      {
        u = u - PARAM_Q;
      }
    //   else
    //   {
    //       break;
    //   }
  }
  return u;
}


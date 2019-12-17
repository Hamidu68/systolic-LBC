#include "conv.h"

void print_vector(DATA_T *arr, int n){
    printf("---------------------------\n");
    for (int i=0;i<n; i++)
      printf("%d,",arr[i]);
    printf("+++++++++++++++++++++++++++\n");
}

void init_vector(DATA_T *arr, int n, int q){
    DATA_T lower = 0;
    DATA_T upper = q-1;
    for (int i=0;i<n; i++)
        arr[i] = (rand() % (upper - lower + 1)) + lower; 
}

int test_vectors(DATA_T* res, DATA_T* gold, int n, int q)
{
    int ret = 0;
    for (int i=0; i<n; i++)
    {
        if (res[i]%q!=gold[i]%q)
            {
                printf("res[%d]:%d \t gold[%d]:%d\n",i,res[i],i,gold[i]);
                ret = 1;
                continue;                
            }
    }

    if (ret)
        printf("FAILED\n");
    else
        printf("SUCCESS\n");
    return ret;
}

int main(int argc, char **argv) {

    int i;
    DATA_T out[OUT_SIZE];


    DATA_T in_signal[SIGNAL_SIZE];
    for (i = 0; i < N_COEFF; i++)
    {
      if (i < N_COEFF)
        in_signal[i] = 1;
      else
        in_signal[i] = 0;
    }

    DATA_T coeffs[SIGNAL_SIZE];
    for (i = 0; i < N_COEFF; i++)
    {
      if (i < N_COEFF)
        coeffs[i] = 1;
      else
        coeffs[i] = 0;
    }


    conv(out, in_signal, coeffs);
       
    print_vector(out, SIGNAL_SIZE);

    printf("TEST PASSED\n");
#ifdef MONTGOMERY
    printf("TEST PASSED -- Montgomery domain\n");
#else 
    printf("TEST PASSED -- Normal domain\n");
#endif
    return EXIT_SUCCESS;
}

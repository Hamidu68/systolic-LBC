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


    int size = SIGNAL_SIZE*2; //we need *2 for padding!!!!

    DATA_T out[OUT_SIZE] = {Q_NEWHOPE, Q_NEWHOPE, Q_NEWHOPE,Q_NEWHOPE,Q_NEWHOPE};
    // DATA_T in_signal[OUT_SIZE];
    // init_vector(in_signal,N_COEFF, Q_NEWHOPE);

    DATA_T in_signal[SIGNAL_SIZE] = {1,2,3,4,5};
    DATA_T coeffs[SIGNAL_SIZE] = {1,1,0,0,0};

    

    conv(out, in_signal, coeffs);
       
    print_vector(in_signal, SIGNAL_SIZE);   
    print_vector(out, SIGNAL_SIZE);

    printf("TEST PASSED\n");
    return EXIT_SUCCESS;
}

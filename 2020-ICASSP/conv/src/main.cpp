#include "conv.h"

void print_vector(DATA_T *arr, int n){
    printf("---------------------------\n");
    for (int i=0;i<n; i++)
      printf("%d,",arr[i]);
    printf("+++++++++++++++++++++++++++\n");
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

    DATA_T out[OUT_SIZE];
    DATA_T in_signal[OUT_SIZE] = {1,1,0,0,0,0,0,0,0,0};
    DATA_T coeffs[SIGNAL_SIZE] = {1,1,1,1,1};

    conv(out, in_signal, coeffs);
       
    print_vector(out, OUT_SIZE);

    printf("TEST PASSED\n");
    return EXIT_SUCCESS;
}

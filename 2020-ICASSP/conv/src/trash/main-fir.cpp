#include <iostream>
using namespace std;
#include "stdio.h"
#include "fir.h"
using namespace std;

int main()
{
    data_t X[SIZE_SIGNAL];
    for (int i = 0; i < SIZE_SIGNAL; i++)
    {
      if (i < N_COEFF)
        X[i] = 1;
      else
        X[i] = 0;
    }

    coef_t H[N_COEFF];
    for (int i = 0; i < N_COEFF; i++)
    {
      if (i < N_COEFF)
        H[i] = 1;
      else
        H[i] = 0;
    }

    data_t out[N_COEFF];

    fir(X, H, out);

    for (int i = 0; i < N_COEFF; i++)
    {
      cout << "LOOP: " << i << " with x " << X[i] ;
      cout << "---" << i << " with out " << out[i] <<endl ;
    }

}
#include <stdint.h>


typedef int16_t data_t;
typedef int16_t coef_t;
typedef int sum_t;



#include<stdio.h>
typedef uint16_t DATA_T;

#define N_COEFF 64
#define SIZE_SIGNAL (N_COEFF*3+2)
/*first output is out at array index of cycle N+2; and the last output is at the index of (N+2)+(2N-1)=N_COEFF*3+1.
so size of output should be N_COEFF*3+2
 */

void fir(data_t X[], coef_t H[], data_t out[] );
uint16_t montgomery_reduce(uint32_t a);


template <typename T0, typename T1, typename T2>
class systolic
{
  private:
    T0 x;
    T2 y0;
    T2 y1;

  public:
    void exec(T0 &x_in, T1 &h, T2 &y_in, T0 &x_out, T2 &y)
    {
        y = y1;
        x_out = x;
        y1 = y0;
        y0 = montgomery_reduce(x * h) + y_in;
        x = x_in;
    }
};
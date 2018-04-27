// See LICENSE for license details.

#include "stdlib.h"
#include "dataset.h"

//--------------------------------------------------------------------------
// vvadd function

void __attribute__((noinline)) vvadd(int coreid, int ncores, size_t n, const data_t* x, const data_t* y, data_t* z)
{
   size_t i;

   // interleave accesses
   for (i = coreid; i < n; i+=ncores)
   {
      z[i] = x[i] + y[i];
   }
}

void __attribute__((noinline)) vvadd_opt(int coreid, int ncores, size_t n, const data_t* x, const data_t* y, data_t* z)
{
  // TODO: Your code here
   size_t i;
   int thread_start = coreid * (n / ncores);
   int thread_end = n;
   if (coreid == 0)
   {
   	 // Core 0 goes 0 - > half
   	 // Core 1 goes half - > end
   	 thread_end = (n / ncores);
   }

   for (i = thread_start; i < thread_end; i+=8)
   {
   	  // Loop unrolling 8
      z[i] = x[i] + y[i];               
      z[i+1] = x[i+1] + y[i+1];
      z[i+2] = x[i+2] + y[i+2];
      z[i+3] = x[i+3] + y[i+3];          
      z[i+4] = x[i+4] + y[i+4];               
      z[i+5] = x[i+5] + y[i+5];
      z[i+6] = x[i+6] + y[i+6];
      z[i+7] = x[i+7] + y[i+7];                
   }


}

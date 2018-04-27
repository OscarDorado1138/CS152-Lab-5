// See LICENSE for license details.

#include "dataset.h"
#include "util.h"
#include <stddef.h>

void matmul(const size_t coreid, const size_t ncores, const size_t lda,  const data_t A[], const data_t B[], data_t C[])
{
  size_t i, j, k;

  for (i = 0; i < lda; i++) {
    for (j = coreid; j < lda; j += ncores) {
      data_t sum = 0;
      for (k = 0; k < lda; k++)
        sum += A[j*lda + k] * B[k*lda + i];
      C[i + j*lda] = sum;
    }
  }
}

void matmul_opt(const size_t coreid, const size_t ncores, const size_t lda,  const data_t A[], const data_t B[], data_t C[])
{

  // TODO: Your code here
   size_t i, j, k;
   int thread_start = coreid * (lda / ncores);
   int thread_end = lda;
   if (coreid == 0)
   {
   	 // Core 0 goes 0 - > half
   	 // Core 1 goes half - > end
   	 thread_end = (lda / ncores);
   }

	// Code transposes B matrix
    data_t transpose_B [lda*lda];
    size_t transpose_max = lda*lda;
	for(size_t t_i = 0; t_i < transpose_max; t_i++)
	{
		int transpose_i = t_i / lda;
		int transpose_j = t_i % lda;
		transpose_B[transpose_j*lda + transpose_i] = B[transpose_i*lda + transpose_j];
	}

	// Flip i and k in indicies of transpose B matrix
	for (j = thread_start; j < thread_end; j++) {
	  for (i = 0; i < lda; i++) {
	      data_t sum = 0;

	      size_t index_A = j*lda;
	      size_t index_B = i*lda;
	      
	      for (k = 0; k < lda; k+=8)
	      {
	        sum += A[index_A + k  ] * transpose_B[index_B +   k];
	        sum += A[index_A + k+1] * transpose_B[index_B + k+1];
	        sum += A[index_A + k+2] * transpose_B[index_B + k+2];
	        sum += A[index_A + k+3] * transpose_B[index_B + k+3];
	        sum += A[index_A + k+4] * transpose_B[index_B + k+4];
	        sum += A[index_A + k+5] * transpose_B[index_B + k+5];
	        sum += A[index_A + k+6] * transpose_B[index_B + k+6];
	        sum += A[index_A + k+7] * transpose_B[index_B + k+7];
	      }
	      C[i + j*lda] = sum;
	    }
	  }

}

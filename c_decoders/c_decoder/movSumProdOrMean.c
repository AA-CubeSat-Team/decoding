/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: movSumProdOrMean.c
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 30-Mar-2022 22:13:07
 */

/* Include Files */
#include "movSumProdOrMean.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const double x[512989]
 *                double y[512989]
 * Return Type  : void
 */
void vmovfun(const double x[512989], double y[512989])
{
  double b_y;
  int b_k;
  int iLeft;
  int k;
  int vlen;

#pragma omp parallel for \
 num_threads(omp_get_max_threads()) \
 private(iLeft,b_k,vlen,b_y)

  for (k = 0; k < 512989; k++) {
    if (k + 1 <= 50) {
      iLeft = -49;
    } else {
      iLeft = k - 99;
    }

    if (k + 50 > 512989) {
      b_k = 512940;
    } else {
      b_k = k + 1;
    }

    vlen = b_k - iLeft;
    if (vlen == 0) {
      b_y = 0.0;
    } else {
      b_y = x[iLeft + 49];
      for (b_k = 2; b_k <= vlen; b_k++) {
        b_y += x[(iLeft + b_k) + 48];
      }
    }

    y[k] = b_y / (double)vlen;
  }
}

/*
 * File trailer for movSumProdOrMean.c
 *
 * [EOF]
 */

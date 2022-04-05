/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: c_decoder_terminate.c
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 30-Mar-2022 22:13:07
 */

/* Include Files */
#include "c_decoder_terminate.h"
#include "c_decoder_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void c_decoder_terminate(void)
{
  omp_destroy_nest_lock(&emlrtNestLockGlobal);
  isInitialized_c_decoder = false;
}

/*
 * File trailer for c_decoder_terminate.c
 *
 * [EOF]
 */

/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: c_decoder_emxutil.h
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 30-Mar-2022 22:13:07
 */

#ifndef C_DECODER_EMXUTIL_H
#define C_DECODER_EMXUTIL_H

/* Include Files */
#include "c_decoder_types.h"
#include "rtwtypes.h"
#include "omp.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  extern void emxEnsureCapacity_real_T(emxArray_real_T *emxArray, int oldNumel);
  extern void emxFree_real_T(emxArray_real_T **pEmxArray);
  extern void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);

#ifdef __cplusplus

}
#endif
#endif

/*
 * File trailer for c_decoder_emxutil.h
 *
 * [EOF]
 */

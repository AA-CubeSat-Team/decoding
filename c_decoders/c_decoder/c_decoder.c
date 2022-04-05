/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: c_decoder.c
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 30-Mar-2022 22:13:07
 */

/* Include Files */
#include "c_decoder.h"
#include "c_decoder_data.h"
#include "c_decoder_emxutil.h"
#include "c_decoder_initialize.h"
#include "c_decoder_types.h"
#include "movSumProdOrMean.h"
#include "rt_nonfinite.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static int div_nde_s32_floor(int numerator, int denominator);
static double rt_atan2d_snf(double u0, double u1);
static double rt_hypotd_snf(double u0, double u1);

/* Function Definitions */
/*
 * Arguments    : int numerator
 *                int denominator
 * Return Type  : int
 */
static int div_nde_s32_floor(int numerator, int denominator)
{
  int b_numerator;
  if (((numerator < 0) != (denominator < 0)) && (numerator % denominator != 0))
  {
    b_numerator = -1;
  } else {
    b_numerator = 0;
  }

  return numerator / denominator + b_numerator;
}

/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_atan2d_snf(double u0, double u1)
{
  double y;
  int b_u0;
  int b_u1;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    if (u0 > 0.0) {
      b_u0 = 1;
    } else {
      b_u0 = -1;
    }

    if (u1 > 0.0) {
      b_u1 = 1;
    } else {
      b_u1 = -1;
    }

    y = atan2(b_u0, b_u1);
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_hypotd_snf(double u0, double u1)
{
  double a;
  double y;
  a = fabs(u0);
  y = fabs(u1);
  if (a < y) {
    a /= y;
    y *= sqrt(a * a + 1.0);
  } else if (a > y) {
    y /= a;
    y = a * sqrt(y * y + 1.0);
  } else {
    if (!rtIsNaN(y)) {
      y = a * 1.4142135623730951;
    }
  }

  return y;
}

/*
 * Decoder for SOC-i's Telemetry Beacon
 *  This file loads in a wav file and decodes it. Instructions can be found
 *  be on the AACT's github (https://github.com/AA-CubeSat-Team/decoding),
 *  and more information about SOC-i can be found at the AACT website
 *  (AACT.space). Please use the "Run Section" button to run the sections
 *  below separately.
 *
 *  Nick Melville
 *  UW Aero Astro Cubesat Team
 * Arguments    : const double IQ_data[1025980]
 *                double telem[432]
 * Return Type  : void
 */
void c_decoder(const double IQ_data[1025980], double telem[432])
{
  static creal_T s[512990];
  static double ang[512989];
  static double b_y[512989];
  static double mag[512989];
  static double y[512989];
  emxArray_real_T *b_ang;
  double IQ_data_re;
  double IQ_data_re_tmp;
  double b_re;
  double mag_mean;
  double re;
  int b_ind_data[1027];
  int b_ind_size_idx_1;
  int b_l;
  int l;
  int tend;
  int tstart;
  signed char bits_data[1027];
  signed char m[161];
  signed char preamble[80];
  signed char x[80];
  unsigned char c_re;
  boolean_T exitg1;
  if (!isInitialized_c_decoder) {
    c_decoder_initialize();
  }

  /*  Load in file */
  /*  Please comment out the file that you want to decode. Also note that the */
  /*  file must be in your matlab path. */
  /* clear all; clc;  */
  /* IQ_data = audioread(file); */
  /*  Plot to determine where the packet starts and ends, if the wav file is */
  /*  not already trimmed to only include the packet. */
  /* plot(IQ_data) */
  /*  Trim the file */
  /*  To reduce unnecessary computations, the wav file should be trimmed to get */
  /*  rid of long periods of silence before and after the packet transmission. */
  /*  The packet is indicated by the obvious spike in the plot of the IQ data.  */
  /*  This step is not necessary if the file is already trimmed using another  */
  /*  software, such as audacity.  */
  /* IQ_data = audioread(file); */
  /*  Adjust these values as needed to only include one packet */
  /* IQ_data = IQ_data(start_ind:end_ind,:); */
  /* plot(IQ_data) */
  /*  Check Center Frequency */
  /*  If is often necessary to shift the frequency to the expected frequency of  */
  /*  435.55 MHz. This might be the case if there is doppler shift, or if your */
  /*  SDR setup has bias. It is a good idea to check that the frequency is */
  /*  centered. */
  /*  The sampling frequency of the SDR (change if needed) */
  /*  Convert to complex number */
  /*  Find center frequency using freqz or some other software such as Audacity */
  /*  (freqz requires the signal processing toolbox) */
  /* freqz(s,1,[-10E5:1E3:10E5],fs); */
  /*  Shift the Center Frequency to Zero */
  /*  Define how far the center frequency needs to be shifted (change as */
  /*  needed). */
  /*  Shift the IQ signal */
  for (tend = 0; tend < 512990; tend++) {
    mag_mean = -1.8723892215395167E+6 * ((double)tend + 1.0) / 2.4E+6;
    re = cos(mag_mean);
    mag_mean = sin(mag_mean);
    IQ_data_re_tmp = IQ_data[tend + 512990];
    IQ_data_re = IQ_data[tend] + 0.0 * IQ_data_re_tmp;
    b_re = IQ_data_re * re - IQ_data_re_tmp * mag_mean;
    mag_mean = IQ_data_re * mag_mean + IQ_data_re_tmp * re;
    s[tend].re = b_re;
    s[tend].im = mag_mean;
  }

  /*  Recheck center frequency (it should be centered now) */
  /* freqz(s,1,[-10E5:1E3:10E5],fs); */
  /*  Demodulate */
  for (l = 0; l < 512989; l++) {
    mag_mean = s[l].re;
    IQ_data_re = -s[l].im;
    b_re = s[l + 1].re;
    IQ_data_re_tmp = s[l + 1].im;
    re = mag_mean * b_re - IQ_data_re * IQ_data_re_tmp;
    mag_mean = mag_mean * IQ_data_re_tmp + IQ_data_re * b_re;
    y[l] = rt_atan2d_snf(mag_mean, re);
    b_y[l] = rt_hypotd_snf(re, mag_mean);
  }

  vmovfun(y, ang);
  vmovfun(b_y, mag);
  mag_mean = mag[0];
  for (tend = 0; tend < 512988; tend++) {
    mag_mean += mag[tend + 1];
  }

  mag_mean /= 512989.0;
  tstart = 1;
  l = 0;
  exitg1 = false;
  while ((!exitg1) && (l < 512989)) {
    if (mag[l] > 0.8 * mag_mean) {
      tstart = l + 1;
      exitg1 = true;
    } else {
      l++;
    }
  }

  tend = 512989;
  l = 0;
  exitg1 = false;
  while ((!exitg1) && (l <= 512989 - tstart)) {
    b_l = tstart + l;
    if (mag[b_l - 1] < 0.8 * mag_mean) {
      tend = b_l;
      exitg1 = true;
    } else {
      l++;
    }
  }

  if (tstart > tend) {
    b_l = 0;
    tstart = -1;
  } else {
    b_l = tstart - 1;
    tstart = tend - 1;
  }

  emxInit_real_T(&b_ang, 1);
  vmovfun(y, b_y);
  tend = tstart - b_l;
  tstart = b_ang->size[0];
  b_ang->size[0] = tend + 1;
  emxEnsureCapacity_real_T(b_ang, tstart);
  for (tstart = 0; tstart <= tend; tstart++) {
    b_ang->data[tstart] = b_y[b_l + tstart];
  }

  mag_mean = 0.0;
  for (l = 0; l < 5000; l++) {
    mag_mean = fmax(mag_mean, b_ang->data[l]);
  }

  tstart = 1;
  l = 0;
  exitg1 = false;
  while ((!exitg1) && (l <= tend)) {
    if (ang[b_l + l] > 0.98 * mag_mean) {
      tstart = l + 1;
      exitg1 = true;
    } else {
      l++;
    }
  }

  if (b_ang->size[0] < tstart) {
    b_ind_size_idx_1 = 0;
  } else {
    b_ind_size_idx_1 = div_nde_s32_floor(b_ang->size[0] - tstart, 500) + 1;
    tend = div_nde_s32_floor(b_ang->size[0] - tstart, 500);
    for (b_l = 0; b_l <= tend; b_l++) {
      b_ind_data[b_l] = tstart + 500 * b_l;
    }
  }

  if (b_ind_size_idx_1 == 0) {
    mag_mean = 0.0;
  } else {
    mag_mean = b_ang->data[b_ind_data[0] - 1];
    for (tend = 2; tend <= b_ind_size_idx_1; tend++) {
      mag_mean += b_ang->data[b_ind_data[tend - 1] - 1];
    }
  }

  mag_mean /= (double)b_ind_size_idx_1;
  if (0 <= b_ind_size_idx_1 - 1) {
    memset(&bits_data[0], 0, b_ind_size_idx_1 * sizeof(signed char));
  }

  for (l = 0; l < b_ind_size_idx_1; l++) {
    if (b_ang->data[b_ind_data[l] - 1] > mag_mean) {
      bits_data[l] = 1;
    } else {
      bits_data[l] = -1;
    }
  }

  emxFree_real_T(&b_ang);

  /*  Decoding */
  for (tend = 0; tend < 20; tend++) {
    tstart = tend << 2;
    preamble[tstart] = 1;
    preamble[tstart + 1] = 1;
    preamble[tstart + 2] = -1;
    preamble[tstart + 3] = -1;
  }

  c_re = 1U;
  for (l = 0; l <= b_ind_size_idx_1 - 80; l++) {
    for (b_l = 0; b_l < 80; b_l++) {
      x[b_l] = (signed char)(bits_data[b_l + l] * preamble[b_l]);
    }

    mag_mean = x[0];
    for (tend = 0; tend < 79; tend++) {
      mag_mean += (double)x[tend + 1];
    }

    if (mag_mean > 76.0) {
      c_re = 0U;
    }
  }

  for (l = 0; l < 3; l++) {
    tstart = (c_re + 160 * l) + 4;
    memcpy(&m[0], &bits_data[tstart], 161U * sizeof(signed char));
    for (tstart = 0; tstart < 160; tstart++) {
      if (m[tstart] == -1) {
        m[tstart] = 0;
      }
    }

    b_l = l * 18;
    for (tend = 0; tend < 8; tend++) {
      for (tstart = 0; tstart < 18; tstart++) {
        telem[(tstart + b_l) + 54 * tend] = m[(tend * 20 + tstart) + 2];
      }
    }
  }

  /*  Parse Telemetry */
}

/*
 * File trailer for c_decoder.c
 *
 * [EOF]
 */

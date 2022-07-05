/*
 * LS_includes.h
 *
 *  Created on: 30-Apr-2022
 *      Author: Ranjith Srinivas
 */

#include "complex.h"

#define NUM_USED_SUB_CARRIERS 52
#define nUSC NUM_USED_SUB_CARRIERS
#define SYMBOLS_USED 200
#define TEST_RUN 200
#define SNR_MAX 30

typedef float complex flt_cmplx;

flt_cmplx trainingSym[nUSC] = {1.0+0.0*I, 1.0+0.0*I, -1.0+0.0*I, -1.0+0.0*I, 1.0+0.0*I, 1.0+0.0*I, -1.0+0.0*I, 1.0+0.0*I, -1.0+0.0*I, 1.0+0.0*I, 1.0+0.0*I, 1.0+0.0*I, 1.0+0.0*I, 1.0+0.0*I, 1.0+0.0*I, -1.0+0.0*I, -1.0+0.0*I, 1.0+0.0*I, 1.0+0.0*I, -1.0+0.0*I, 1.0+0.0*I, -1.0+0.0*I, 1.0+0.0*I, 1.0+0.0*I, 1.0+0.0*I, 1.0+0.0*I, 1.0+0.0*I, -1.0+0.0*I, -1.0+0.0*I, 1.0+0.0*I, 1.0+0.0*I, -1.0+0.0*I, 1.0+0.0*I, -1.0+0.0*I, 1.0+0.0*I, -1.0+0.0*I, -1.0+0.0*I, -1.0+0.0*I, -1.0+0.0*I, -1.0+0.0*I, 1.0+0.0*I, 1.0+0.0*I, -1.0+0.0*I, -1.0+0.0*I, 1.0+0.0*I, -1.0+0.0*I, 1.0+0.0*I, -1.0+0.0*I, 1.0+0.0*I, 1.0+0.0*I, 1.0+0.0*I, 1.0+0.0*I};

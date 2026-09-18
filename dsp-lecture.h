#ifndef DSP_LECTURE_H
#define DSP_LECTURE_H

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <arm_acle.h>
#include <arm_neon.h>
#include <time.h>
#include <stdbool.h>
#include <limits.h>
#include <malloc.h>

#define N_MAX 65535
#define MAX16 32767
#define MIN16 -32768

typedef struct { int16_t real,imag; } cint16_t;
typedef struct { int32_t real,imag; } cint32_t;

void iir_fast(int k, int m, int n, int16_t* dst, int16_t* bwtaps, int16_t* fwtaps, int16_t* src);
void iir_ref(int k, int m, int n, int16_t* dst, int16_t* bwtaps, int16_t* fwtaps, int16_t* src);

#endif /* DSP_LECTURE_H */

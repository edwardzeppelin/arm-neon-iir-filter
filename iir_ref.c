#include "dsp-lecture.h"

void iir_ref(int k, int m, int n, int16_t* dst, int16_t* bwtaps, int16_t* fwtaps, int16_t* src) {

    int16_t* y_tmp = (int16_t*)calloc(n, sizeof(int16_t));
    if (!y_tmp) return;
    
    for (int i = 0; i < n; i++) {
        int64_t res = 0; 
        
        for (int j = 0; j < m; j++) {
            if (i - j >= 0) {
                res += (int64_t)bwtaps[j] * (int64_t)src[i - j];
            }
        }
        
        for (int j = 1; j <= k; j++) {
            if (i - j >= 0) {
                res -= (int64_t)fwtaps[j] * (int64_t)y_tmp[i - j];
            }
        }
        
        res >>= 15;
        
        if (res > INT16_MAX) {
            y_tmp[i] = INT16_MAX;
            dst[i] = INT16_MAX;
        }
        else if (res < INT16_MIN) {
            y_tmp[i] = INT16_MIN;
            dst[i] = INT16_MIN;
        }
        else {
            y_tmp[i] = (int16_t)res;
            dst[i] = (int16_t)res;
        }
    }
    
    free(y_tmp);
}
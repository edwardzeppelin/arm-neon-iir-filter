#include "dsp-lecture.h"

void iir_fast(int k, int m, int n, int16_t* dst, int16_t* bwtaps, int16_t* fwtaps, int16_t* src) {
    int16_t* y_tmp = (int16_t*)calloc(n, sizeof(int16_t));
    if (!y_tmp) return;
    
    for (int i = 0; i < n; i++) {
        int32x4_t forward_acc = vdupq_n_s32(0);
        int32x4_t feedback_acc = vdupq_n_s32(0);
        
        int j = 0;
        for (; j <= m - 8; j += 8) {
            if (i - j - 7 >= 0) {
				
                int16x8_t src_vec = vld1q_s16(&src[i - j - 7]);

                src_vec = vrev64q_s16(src_vec);
                src_vec = vcombine_s16(vget_high_s16(src_vec), vget_low_s16(src_vec));
                
                int16x8_t taps_vec = vld1q_s16(&bwtaps[j]);
                
                forward_acc = vmlal_s16(forward_acc, vget_low_s16(src_vec), vget_low_s16(taps_vec));
                forward_acc = vmlal_s16(forward_acc, vget_high_s16(src_vec), vget_high_s16(taps_vec));
            } else {
                break;
            }
        }
        
        int32x2_t sum = vadd_s32(vget_low_s32(forward_acc), vget_high_s32(forward_acc));
        sum = vpadd_s32(sum, sum);
        int64_t forward = vget_lane_s32(sum, 0);
        
        for (; j < m; j++) {
            if (i - j >= 0) {
                forward += (int64_t)src[i - j] * (int64_t)bwtaps[j];
            }
        }

        j = 1;
        for (; j <= k - 8; j += 8) {
            if (i - j - 7 >= 0) {
                int16x8_t y_vec = vld1q_s16(&y_tmp[i - j - 7]);
                y_vec = vrev64q_s16(y_vec);
                y_vec = vcombine_s16(vget_high_s16(y_vec), vget_low_s16(y_vec));
                
                int16x8_t fw_vec = vld1q_s16(&fwtaps[j]);
                
                feedback_acc = vmlal_s16(feedback_acc, vget_low_s16(y_vec), vget_low_s16(fw_vec));
                feedback_acc = vmlal_s16(feedback_acc, vget_high_s16(y_vec), vget_high_s16(fw_vec));
            } else {
                break;
            }
        }

        sum = vadd_s32(vget_low_s32(feedback_acc), vget_high_s32(feedback_acc));
        sum = vpadd_s32(sum, sum);
        int64_t feedback = vget_lane_s32(sum, 0);
        
        for (; j <= k; j++) {
            if (i - j >= 0) {
                feedback += (int64_t)y_tmp[i - j] * (int64_t)fwtaps[j];
            }
        }
        
        int64_t res = forward - feedback;
        res >>= 15;
        
        int16_t result;
        if (res > INT16_MAX) result = INT16_MAX;
        else if (res < INT16_MIN) result = INT16_MIN;
        else result = (int16_t)res;
        
        y_tmp[i] = result;
        dst[i] = result;
    }
    
    free(y_tmp);
}
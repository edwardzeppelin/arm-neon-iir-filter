#include "dsp-lecture.h"

void iir_fast(int k, int m, int n, int16_t* dst, int16_t* bwtaps, int16_t* fwtaps, int16_t* src) {
    int16_t* y_tmp = (int16_t*)calloc(n, sizeof(int16_t));
    if (!y_tmp) return;
    
    for (int i = 0; i < n; i++) {
        int64_t forward = 0;
        int64_t feedback = 0;
        
        // Прямая часть - векторизация по 4 элемента
        int j = 0;
        for (; j <= m - 4; j += 4) {
            // Проверяем, что все 4 элемента доступны
            if (i - j - 3 >= 0) {
                forward += (int64_t)src[i - j] * (int64_t)bwtaps[j] +
                          (int64_t)src[i - j - 1] * (int64_t)bwtaps[j + 1] +
                          (int64_t)src[i - j - 2] * (int64_t)bwtaps[j + 2] +
                          (int64_t)src[i - j - 3] * (int64_t)bwtaps[j + 3];
            } else {
                break;
            }
        }
        
        // Оставшиеся коэффициенты прямой части
        for (; j < m; j++) {
            if (i - j >= 0) {
                forward += (int64_t)src[i - j] * (int64_t)bwtaps[j];
            }
        }
        
        // Обратная связь
        j = 1;  // Начинаем с 1
        for (; j <= k - 4; j += 4) {
            if (i - j - 3 >= 0) {
                feedback += (int64_t)y_tmp[i - j] * (int64_t)fwtaps[j] +
                           (int64_t)y_tmp[i - j - 1] * (int64_t)fwtaps[j + 1] +
                           (int64_t)y_tmp[i - j - 2] * (int64_t)fwtaps[j + 2] +
                           (int64_t)y_tmp[i - j - 3] * (int64_t)fwtaps[j + 3];
            } else {
                break;
            }
        }
        
        // Оставшиеся коэффициенты обратной связи
        for (; j <= k; j++) {
            if (i - j >= 0) {
                feedback += (int64_t)y_tmp[i - j] * (int64_t)fwtaps[j];
            }
        }
        
        // Общий результат с одним сдвигом
        int64_t res = forward - feedback;
        res >>= 15;
        
        // Ограничение
        int16_t result;
        if (res > INT16_MAX) result = INT16_MAX;
        else if (res < INT16_MIN) result = INT16_MIN;
        else result = (int16_t)res;
        
        y_tmp[i] = result;
        dst[i] = result;
    }
    
    free(y_tmp);
}
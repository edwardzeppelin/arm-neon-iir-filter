#include "dsp-lecture.h"

void random_vector(int n, int16_t* v) {
    for (int i = 0; i < n; i++) {
        v[i] = (int16_t)(rand() % 256);
    }
}

void iir_tb(int seed, int num_runs) {
	
	int run, n, m, k;
	int16_t* dst_ref = NULL;
	int16_t* dst_fast = NULL;
	int16_t* bwtaps = NULL;
	int16_t* fwtaps = NULL;
	int16_t* src = NULL;
	
    srand48(seed);
    
    for (run = 0; run < num_runs; run++) {
		k = rand() % N_MAX;
        m = rand() % N_MAX;
		n = rand() % N_MAX;
        
        int16_t* dst_ref = (int16_t*)calloc(n, sizeof(int16_t));
        int16_t* dst_fast = (int16_t*)calloc(n, sizeof(int16_t));
        int16_t* bwtaps = (int16_t*)calloc(k, sizeof(int16_t));
		int16_t* fwtaps = (int16_t*)calloc(m, sizeof(int16_t));
        int16_t* src = (int16_t*)calloc(n, sizeof(int16_t));
		
		if (!dst_fast || !dst_ref || !src || !bwtaps || !fwtaps) {
			fprintf(stderr, "Memory allocation error.\n");
			free(dst_fast);
			free(dst_ref);
			free(bwtaps);
			free(fwtaps);
			free(src);
			return;
		}
		
        random_vector(k, bwtaps);
		random_vector(m, fwtaps);
        random_vector(n, src);

        clock_t ref_start = clock();
        iir_ref(k, m, n, dst_ref, bwtaps, fwtaps, src);
        clock_t ref_time = clock() - ref_start;
        printf("Ref time: %ld ticks\n", (long)ref_time);

        clock_t fast_start = clock();
        iir_fast(k, m, n, dst_fast, bwtaps, fwtaps, src);
        clock_t fast_time = clock() - fast_start;
        printf("Fast time: %ld ticks\n", (long)fast_time);
     
        bool error = false;
        for (int i = 0; i < n; i++) {
            if (dst_fast[i] != dst_ref[i]) {
                fprintf(stderr, "Error at index %d: ref=(%d) fast=(%d)\n", i, dst_ref[i], dst_fast[i]);
                error = true;
                break;
            }
        }
        
        if (!error) {
            printf("Run %d: OK (n=%d, m=%d)\n", run, n, m);
        }

        free(src);
        free(bwtaps);
		free(fwtaps);
        free(dst_fast);
        free(dst_ref);
    }
}

int main() {
    fir_tb(1316, 100);
    printf("Success\n");
    return 0;
}
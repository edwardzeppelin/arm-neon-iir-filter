# ARM NEON Q15 IIR Filter Optimization

Performance benchmark and verification suite for a 16-bit Q15 **IIR (Infinite Impulse Response) Filter** accelerated using **ARM NEON SIMD intrinsics**.

## Key Features
* **Q15 Fixed-Point IIR Filtering**: Computes output samples $y[i] = \sum_{j=0}^{m-1} b_j \cdot x[i-j] - \sum_{j=1}^{k} a_j \cdot y[i-j]$ with Q15 scaling (`>> 15`) and 16-bit saturation clamping (`INT16_MAX` / `INT16_MIN`).
* **ARM NEON SIMD Acceleration**:
  * Dual-path vectorization for feedforward (`bwtaps`) and feedback (`fwtaps`) filter coefficients.
  * Reverses vector sample order using `vrev64q_s16` and `vcombine_s16` to align time-reversed delay lines.
  * Multiply-accumulate operations using `vmlal_s16` into 32-bit vector accumulators (`int32x4_t`).
  * Fast vector sum reduction with `vadd_s32` and `vpadd_s32`.
* **State & Boundary Management**: Dynamic history buffer (`y_tmp`) for recursive feedback computation and scalar fallback loops for non-aligned vector tails.
* **Automated Verification**: Testbench measures execution speed (`clock()`) and validates fast NEON results against scalar C reference calculations.

## Project Structure
* `iir_fast.c` — Optimized IIR filter using ARM NEON SIMD intrinsics.
* `iir_ref.c` — Scalar reference C implementation of the IIR filter.
* `iir_tb.c` — Benchmark testbench with randomized signal inputs and execution timer.
* `dsp-lecture.h` — Header declaring function prototypes, structures, and limits (`N_MAX`, `MAX16`, `MIN16`).

## Build & Run

```bash
gcc -O2 -mfpu=neon iir_tb.c iir_fast.c iir_ref.c -o neon_iir_test
./neon_iir_test
```

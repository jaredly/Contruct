// mtrand.cpp, see include file mtrand.h for information

#include "stdafx.h"
// non-inline function definitions and static member definitions cannot
// reside in header file because of the risk of multiple declarations

// initialization of static private members
unsigned long MTRand_int32::state[MTRAND_N] = {0x0UL};
int MTRand_int32::p = 0;
bool MTRand_int32::init = false;

void MTRand_int32::gen_state() { // generate new state vector
	int i;
  for (i = 0; i < (MTRAND_N - MTRAND_M); ++i)
    state[i] = state[i + MTRAND_M] ^ twiddle(state[i], state[i + 1]);
  for (i = MTRAND_N - MTRAND_M; i < (MTRAND_N - 1); ++i)
    state[i] = state[i + MTRAND_M - MTRAND_N] ^ twiddle(state[i], state[i + 1]);
  state[MTRAND_N - 1] = state[MTRAND_M - 1] ^ twiddle(state[MTRAND_N - 1], state[0]);
  p = 0; // reset position
}

void MTRand_int32::seed(unsigned long s) {  // init by 32 bit seed
  state[0] = s & 0xFFFFFFFFUL; // for > 32 bit machines
  for (int i = 1; i < MTRAND_N; ++i) {
    state[i] = 1812433253UL * (state[i - 1] ^ (state[i - 1] >> 30)) + i;
// see Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier
// in the previous versions, MSBs of the seed affect only MSBs of the array state
// 2002/01/09 modified by Makoto Matsumoto
    state[i] &= 0xFFFFFFFFUL; // for > 32 bit machines
  }
  p = MTRAND_N; // force gen_state() to be called for next random number
}

void MTRand_int32::seed(const unsigned long* array, int size) { // init by array
  seed(19650218UL);
  int i = 1, j = 0, k;
  for (k = ((MTRAND_N > size) ? MTRAND_N : size); k; --k) {
    state[i] = (state[i] ^ ((state[i - 1] ^ (state[i - 1] >> 30)) * 1664525UL))
      + array[j] + j; // non linear
    state[i] &= 0xFFFFFFFFUL; // for > 32 bit machines
    ++j; j %= size;
    if ((++i) == MTRAND_N) { state[0] = state[MTRAND_N - 1]; i = 1; }
  }
  for (k = MTRAND_N - 1; k; --k) {
    state[i] = (state[i] ^ ((state[i - 1] ^ (state[i - 1] >> 30)) * 1566083941UL)) - i;
    state[i] &= 0xFFFFFFFFUL; // for > 32 bit machines
    if ((++i) == MTRAND_N) { state[0] = state[MTRAND_N - 1]; i = 1; }
  }
  state[0] = 0x80000000UL; // MSB is 1; assuring non-zero initial array
  p = MTRAND_N; // force gen_state() to be called for next random number
}


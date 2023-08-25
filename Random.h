

#ifndef NUNU_RANDOM_H
#define NUNU_RANDOM_H

// #include "./Set.h"
#include "./List.h"
#include <chrono>
// #include <cstdint>
#include <stdint.h>

inline uint64_t CurrentTime_milliseconds() 
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
}

inline uint64_t CurrentTime_microseconds() 
{
    return std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
}

inline uint64_t CurrentTime_nanoseconds()
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
}


// adapted to this project by D. Lemire, from https://github.com/wangyi-fudan/wyhash/blob/master/wyhash.h
// This uses mum hashing.
// #include <stdint.h>
// #include <chrono>
// #include <ctime>    // For time()

// state for wyhash64
uint64_t wyhash64_x; /* The state can be seeded with any value. */

// call wyhash64_seed before calling wyhash64
static inline void wyhash64_seed(uint64_t seed) { wyhash64_x = seed; }

static inline uint64_t wyhash64_stateless(uint64_t *seed) {
	*seed += UINT64_C(0x60bee2bee120fc15);
	__uint128_t tmp;
	tmp = (__uint128_t)*seed * UINT64_C(0xa3b195354a39b70d);
	uint64_t m1 = (tmp >> 64) ^ tmp;
	tmp = (__uint128_t)m1 * UINT64_C(0x1b03738712fad5c9);
	uint64_t m2 = (tmp >> 64) ^ tmp;
	return m2;
}

static inline uint64_t wyhash64_byvalue(uint64_t seed) {
	seed += UINT64_C(0x60bee2bee120fc15);
	__uint128_t tmp;
	tmp = (__uint128_t)seed * UINT64_C(0xa3b195354a39b70d);
	uint64_t m1 = (tmp >> 64) ^ tmp;
	tmp = (__uint128_t)m1 * UINT64_C(0x1b03738712fad5c9);
	uint64_t m2 = (tmp >> 64) ^ tmp;
	return m2;
}

// returns random number, modifies wyhash64_x
static inline uint64_t wyhash64(void) { return wyhash64_stateless(&wyhash64_x); }

// returns the 32 least significant bits of a call to wyhash64
// this is a simple function call followed by a cast
static inline uint32_t wyhash64_cast32(void) { return (uint32_t)wyhash64(); }

static inline uint64_t random_generator(){
	// wyhash64_seed(CurrentTime_nanoseconds());
	return wyhash64_byvalue((uint64_t)CurrentTime_nanoseconds());
	// return wyhash64();
}

static inline double random_unit(){
	return (double)random_generator()/UINT64_MAX;
}

int randrange(int ubound, int lbound=0){
	return (int)(random_generator()%(ubound - lbound)) + lbound;
}

int uniform(int ubound){
	return (int)(random_generator()%(ubound));
}

template <typename T>
T choice(List<T> lst){
	// return random element of lst.
	return lst[uniform(lst.length())];
}

template <typename T>
void shuffle(List<T>& lst){
	// Randomly shuffly list lst
	int j;
	int n= lst.length();
	T tmp_swap;
	for(int i=0; i<n-1; ++i){
		j = i+uniform(n-i);
		tmp_swap = lst[i];
		lst[i] = lst[j];
		lst[j] = tmp_swap;
	}
}

List<int> permutation(int n){
  // return a shuffled list of [0 : n-1]
  List<int> lst(n);
  for(int i=0; i<n; ++i){
    lst[i] = i;
  }
  shuffle(lst);
  return lst;
}


template <typename T>
List<int> sample(List<int>& lst, int len){
  //  LEN CANNOT BE LARGER THAN LST.LENGHT()
  int n = lst.length();
  if(n>len){
    throw std::invalid_argument("len is larger than lst.length().");
  }
  int j;
  T tmp_swap;
  List<T> result(len);
  for(int i=0; i<len-1; ++i){
    j = i+uniform(n-i);
    tmp_swap = lst[i];
    lst[i] = lst[j];
    lst[j] = tmp_swap;
  }
  return result;
}


#endif  /* CURRENT_TIME_H */
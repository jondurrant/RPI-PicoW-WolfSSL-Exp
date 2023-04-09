#include "pico/stdlib.h"


unsigned long my_time(unsigned long* timer){
	return to_us_since_boot(get_absolute_time());
}



//https://deemocean.com/2022/10/02/rp2040-randomness-and-ring-oscillator/

#define ROSC_RANDOMBIT_OFFSET _u(0x0000001c)
#define ROSC_BASE _u(0x40060000)

// return random number by ROSC between 0-2^N
// N should be in [0,32]
unsigned int ROrand(int N)
{
  static volatile uint32_t *randbit_reg = (uint32_t *)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);
  unsigned int random = 0;
  for (int i = 0; i < N; i++)
  {
    unsigned int random_bit = 0x1 & (*randbit_reg);
    random = random << 1 | random_bit;
  }
  return random;
}


unsigned int my_rng_seed_gen(void){
	return ROrand(32);

}



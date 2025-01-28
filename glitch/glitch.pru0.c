#include <stdint.h>
#include <stdbool.h>
#include <pru_cfg.h>

#include "resource_table_empty.h"
#include "prugpio.h"

#define PRU0_DRAM 0x00000 /* Offset to DRAM */ 

/*  Skip the first 0x200 byte of DRAM since the Makefile allocates
    0x100 for the STACK and 0x100 for the HEAP. */
volatile unsigned int *pru0_dram = (unsigned int *) (PRU0_DRAM + 0x200);

volatile register uint32_t __R30;
volatile register uint32_t __R31;

void main(void) {
  bool reset_pulse = false;

  uint32_t pulse_delay_counter = 0;
  uint32_t pulse_width_counter = 0;
  
  uint32_t gpio_p9_27_in = P9_27;
  uint32_t gpio_p9_30_out = P9_30;
	
  /* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
  CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

  /* initialize dram values */
  pru0_dram[0] = 0;   /* pulse delay */
  pru0_dram[1] = 200; /* pulse width */
  
  while(1) {
    if( (__R31 & gpio_p9_27_in) == gpio_p9_27_in ) {
      reset_pulse = true;      
    } else {
      if( reset_pulse ) {

	/* perform read on dram */
	pulse_delay_counter = pru0_dram[0];
	pulse_width_counter = pru0_dram[1];

	if( pulse_delay_counter > 0 ) {
	  while( pulse_delay_counter > 0 ) {
	    __delay_cycles(1);
	    pulse_delay_counter--;
	  }
	}
	__R30 |= gpio_p9_30_out; /* HIGH */

	while( pulse_width_counter > 0 ) {
	  __delay_cycles(1);
	  pulse_width_counter--;
	}
	__R30 &= ~gpio_p9_30_out; /* LOW */
	reset_pulse = false;
      }
    }
  }
}

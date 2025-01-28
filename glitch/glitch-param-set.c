/* 
 *
 *  glitch-param-set
 *	Stores glitch parameters stored in the PRU's Data memory
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

#define PRU_ADDR		0x4A300000		// Start of PRU memory Page 184 am335x TRM
#define PRU_LEN			0x80000			// Length of PRU memory
#define PRU0_DRAM		0x00000			// Offset to DRAM
#define PRU1_DRAM		0x02000
#define PRU_SHAREDMEM	        0x10000			// Offset to shared memory

unsigned int	*pru0DRAM_32int_ptr;		// Points to the start of local DRAM
unsigned int	*pru1DRAM_32int_ptr;		// Points to the start of local DRAM
unsigned int	*prusharedMem_32int_ptr;	// Points to the start of the shared memory

int pru0_dram_write(uint32_t pulse_delay_cycles, uint32_t pulse_width_cycles) {
	unsigned int *pruDRAM_32int_ptr = pru0DRAM_32int_ptr;

	printf("pulse_delay_cycles: %d, pulse_width_cycles: %d\n",
	       pulse_delay_cycles, pulse_width_cycles);
	
	/* write to PRU shared memory */
	pruDRAM_32int_ptr[0] = pulse_delay_cycles;
	pruDRAM_32int_ptr[1] = pulse_width_cycles;
	return 0;
}

void print_usage() {
  printf("Usage: glitch-param-set [options] [arg]\n\n");
  printf("Options:\n"
	 " -h,   --help     displays this menu\n"
         " -d,  --delay     number of pru cycles to delay glitch pulse, default: 0\n"
         " -w,  --width     width of glitch pulse in pru cycles,        default: 200\n"
         "\n\n");
}

int main(int argc, char *argv[]) {
	unsigned int *pru;
	int fd, c;
	char *arg_flag, *arg_delay, *arg_width = NULL;

	uint32_t pulse_delay_cycles = 0;
	uint32_t pulse_width_cycles = 200;
	
	if( argc > 1 ) {
	  if( !strcmp(argv[1], "-h") ||
	      !strcmp(argv[1], "--help") ) {
	    print_usage();
	    return 0;
	  }
	}

	/* process command-line input */
	for(c=1; c<argc; c++)
	  {
	    arg_flag = argv[c];
	    if( arg_flag != NULL )
	      {		
		if( !strcmp(arg_flag,"-d") ||
		    !strcmp(arg_flag,"--delay")) {
		  arg_delay = argv[c+1];
		}
		
		if( !strcmp(arg_flag,"-w") ||
		    !strcmp(arg_flag,"--width")) {
		  arg_width = argv[c+1];
		}

		arg_flag=NULL;
	      }
	  }
	
	if( arg_delay != NULL )
	  pulse_delay_cycles = atoi(arg_delay);
	if( arg_width != NULL )
	  pulse_width_cycles = atoi(arg_width);

	if( pulse_width_cycles == 0 )
	  pulse_width_cycles = 1;
	
	fd = open ("/dev/mem", O_RDWR | O_SYNC);
	if (fd == -1) {
		printf ("ERROR: could not open /dev/mem.\n\n");
		return 1;
	}
	pru = mmap (0, PRU_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PRU_ADDR);
	if (pru == MAP_FAILED) {
		printf ("ERROR: could not map memory.\n\n");
		return 1;
	}

	printf ("Using /dev/mem.\n");
	
	pru0DRAM_32int_ptr =     pru + PRU0_DRAM/4 + 0x200/4;	/* Points to 0x200 of PRU0 memory */
	pru1DRAM_32int_ptr =     pru + PRU1_DRAM/4 + 0x200/4;	/* Points to 0x200 of PRU1 memory */
	prusharedMem_32int_ptr = pru + PRU_SHAREDMEM/4;	        /* Points to start of shared memory */

	pru0_dram_write(pulse_delay_cycles, pulse_width_cycles);
	
	if(munmap(pru, PRU_LEN)) {
		printf("munmap failed\n");
	} else {
		printf("munmap succeeded\n");
	}
}

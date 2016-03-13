#include <stdio.h>
#include <stdlib.h>

#include "libdune/dune.h"
#include "libdune/cpu-x86.h"

static void recover(void)
{
	printf("hello: recovered from divide by zero\n");
	exit(0);
}

static void divide_by_zero_handler(struct dune_tf *tf)
{
	printf("hello: caught divide by zero!\n");
	tf->rip = (uintptr_t) &recover;
}

static void gp_handler(struct dune_tf *tf)
{
	printf("hello: caught general-protection exception\n");
	tf->rip = (uintptr_t) &recover;
}

int main(int argc, char *argv[])
{
	volatile int ret;

	printf("hello: not running dune yet\n");

	ret = dune_init_and_enter();
	if (ret) {
		printf("failed to initialize dune\n");
		return ret;
	}

	printf("hello: now printing from dune mode\n");

	dune_register_intr_handler(T_DIVIDE, divide_by_zero_handler);
	dune_register_intr_handler(T_GPFLT, gp_handler);

	unsigned int low, high;
	unsigned int msr = 0x00000481H;
	//asm volatile("rdmsr" :"=a" (low),"=d" (high):"c" (msr));
	printf("edx is:%x, eax is: %x\n", low, high);


	ret = 1 / ret; /* divide by zero */

	printf("hello: we won't reach this call\n");

	return 0;
}


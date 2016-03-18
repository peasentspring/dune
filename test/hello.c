#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/syscall.h>

#include "libdune/dune.h"
#include "libdune/cpu-x86.h"

static int check_dune(void)
{
	dune_flush_tlb();

	return 0;
}

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

static void fork_handler(struct dune_tf * tf)
{
	printf("no fork, add by wenjia zhao");
//	tf->rip = (uintprt_t) &recover;
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
	dune_register_syscall_handler(fork_handler);
	
	//unsigned int low, high;
	//unsigned int msr = 0x00000481H;
	////asm volatile("rdmsr" :"=a" (low),"=d" (high):"c" (msr));
	//printf("edx is:%x, eax is: %x\n", low, high);
	//
	//
	//ret = 1 / ret; /* divide by zero */
	//
	//printf("hello: we won't reach this call\n");
	time_t t;
	time(&t);
	syscall(2);
//	int pid = fork();
//	if (pid < 0){
//		 printf("fork create error\n");
//	} else if (pid == 0){
//	//child
//	   if (dune_enter())
//		return 1;
//	   printf("%d\n", 1/0);
//	   if (check_dune())
//		exit(1);
//	   char *const hello_mine_args[] = {"hello_mine", NULL};
//	   int err = execv("/home/peasant/Go/Docker/dune/test/hello_mine", hello_mine_args); 
//	   if (err < 0) {
//	      	perror("execvp has error\n");
//	   }
//	}  
//	if (waitpid(pid, NULL, 0) < 0){
//		printf("wait error\n");
//	}

	return 0;
}


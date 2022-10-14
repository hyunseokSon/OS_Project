#include "types.h"
#include "stat.h"
#include "user.h"

#define PNUM 5 // Process 개수
#define PRINT_CYCLE 10000000 // Process가 정보를 출력하는 주기
#define TOTAL_COUNTER 500000000 // Process가 종료할 때 counter 값

void sdebug_func(void)
{
	int n, pid;

	printf(1, "start sdebug command\n");

	for (n=0; n < PNUM ; n++) 
	{
		pid = fork();

		if (pid < 0)
		{
			printf(1, "ERROR : fork failed...\n");
			break;
		}

		if (pid == 0) // child process인 경우.
		{
			long counter = 0;
			int first = 1;
			weightset(n+1);

			int start_time = uptime();
			int print_counter = PRINT_CYCLE;

			while (counter < TOTAL_COUNTER)
			{
				if (print_counter ==0)
				{
					if (first)
					{
						int end_time = uptime();
						printf(1, "PID : %d, WEIGHT : %d, ", getpid(), weightset(n+1));
						printf(1, "TIMES : %d ms\n", (end_time - start_time) * 10);
						first = 0;
					}
					print_counter = PRINT_CYCLE;
				}
				print_counter--;
				counter++;
			}

			printf(1, "PID : %d terminated\n", getpid());
			exit();
		}
	}

	// parent process는 child process를 wait해줘야 함.
	for(; n> 0; n--)
	{
		if(wait() <0) 
		{
			printf(1, "wait stopped early\n");
			exit();
		}
	}

	if (wait() != -1)
	{
		printf(1, "wait got too many\n");
		exit();
	}

	printf(1, "end of sdebug command\n");
}

int main(void)
{
	sdebug_func();
	exit();
}

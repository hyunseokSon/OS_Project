#include "types.h"
#include "stat.h"
#include "user.h"

#define PNUM 5 // Process 개수
#define PRINT_CYCLE 100000000 // Process가 정보를 출력하는 주기
#define TOTAL_COUNTER 500000000 // Process가 종료할 때 counter 값

void sdebug_func(void)
{
	int n, pid;
	long start_time=0, end_time=0;
	int weight = 1;
	long counter =0;

	printf(1, "start sdebug command\n");

	for (n=0; n < PNUM ; n++) 
	{
		pid = fork();

		if (pid < 0)
			break;

		else if (pid == 0) // child process인 경우.
		{
			start_time = uptime();
			weightset(weight);

			while(1) 
			{
				counter++;

				if (counter == PRINT_CYCLE)
				{
					end_time = uptime();
					printf(1, "PID = %d, WEIGHT: %d, TIMES = %d ms\n", getpid(), weight, (end_time - start_time) * 10);
				}

				if (counter == TOTAL_COUNTER)
				{
					exit();
				}
			}
		}
		weight++;
	}
			/*weightset(n+1);

			for (long long counter=0; counter < TOTAL_COUNTER ; counter++)
			{
				if(counter == PRINT_CYCLE)
				{
					end_time = uptime();
					printf(1, "PID = %d, WEIGHT: %d, TIMES = %d ms\n", getpid(), n+1, (end_time - start_time) * 10);
				}
			}

			exit();
		}

		else // error..
		{
			printf(2, "\n Error... \n");
			break;
		}
		*/

	// parent process는 child process를 wait해줘야 함.
	for(; n> 0; n--)
	{
		pid = wait();
		if (pid <0) 
		{
			printf(1, "wait stopped early\n");
			exit();
		}

		printf(1, "PID : %d terminated\n", pid);
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

#include "types.h"
#include "stat.h"
#include "user.h"

#define PNUM 5 // Process 개수
#define PRINT_CYCLE 100000000 // Process가 정보를 출력하는 주기
#define TOTAL_COUNTER 500000000 // Process가 종료할 때 counter 값

void sdebug_func(void)
{
	int n, pid;
	int start_time=0, end_time=0;

	printf(1, "start sdebug command\n");

	for (n=0; n < PNUM ; n++) 
	{
		pid = fork();
		if (pid >0) // parent process인 경우.
			continue;

		else if (pid == 0) // child process인 경우.
		{
			start_time = uptime();
			weightset(n+1);

			for (long long counter=0; counter <= TOTAL_COUNTER ; counter++)
			{
				if(counter == PRINT_CYCLE)
				{
					end_time = uptime();
					printf(1, "PID = %d, WEIGHT: %d, TIMES = %d ms\n", getpid(), n+1, (end_time - start_time) * 10);
				}

				if (counter == TOTAL_COUNTER) {
					printf(1, "PID : %d terminated\n", getpid());
					exit();
				}
			}
		}

		else // error..
		{
			printf(2, "\n Error... \n");
			break;
		}
	}

	// parent process는 child process를 wait해줘야 함.
	for(; n> 0; n--)
	{
		if (wait() <0)
		{
			printf(1, "wait stopped early\n");
			exit();
		}
	}
	printf(1, "end of sdebug command\n");
}

int main(void)
{
	sdebug_func();
	exit();
}

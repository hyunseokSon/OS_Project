#include "types.h"
#include "stat.h"
#include "user.h"

#define PNUM 5 // Process 개수
#define PRINT_CYCLE 100000000 // Process가 정보를 출력하는 주기
#define TOTAL_COUNTER 500000000 // Process가 종료할 때 counter 값

void sdebug_func(void)
{
	int n, pid;
	printf(1, "start sdebug command\n");

	for (n=0; n < PNUM ; n++) 
	{
		pid = fork();
		if (pid >0) // parent process인 경우.
			continue;

		else if (pid == 0) // child process인 경우.
		{ 
			weightset(n+1);

			for (long long counter=0; counter < TOTAL_COUNTER ; counter++)
			{
				if(counter == PRINT_CYCLE)
				{
					printf(1, "PID = %d, WEIGHT: %d\n", getpid(), n+1);
				}
			}
		}

		else // error..
		{
			printf(2, "\n Error... \n");
			exit();
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
	//wait();
	/*if (pid>0) {
		for(; n>0; n--) {
			wait();
		}
	}
	*/
	printf(1, "end of sdebug command\n");
}

int main(void)
{
	sdebug_func();
	exit();
}

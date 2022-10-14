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

		if (pid < 0) // 20182625 fork가 실패한 경우
		{
			printf(1, "ERROR : fork failed...\n");
			break;
		}

		if (pid == 0) //20182625 child process인 경우.
		{
			int start_time = uptime(); // process 시작 시간

			int counter = 0; // counter는 fork()로 생성된 process가 생성 이후 스케줄되어 수행되는 시간이다.
			int print_counter = PRINT_CYCLE; // process 정보를 출력하기 위한 print_counter
			int first = 1; // 프로세스 정보를 처음 출력하는지 확인하기 위한 변수

			weightset(n+1); //20182625 자식 process 생성 시 weightset()함수 호출. 생성된 순서에 따라 1부터 증가시키며 부여하므로 weightset(n+1)이 됨.
			while (counter < TOTAL_COUNTER)
			{
				print_counter--;

				if (print_counter ==0) // print_counter가
				{
					if (first) //20182625 프로세스 별로 한 번만 출력하는지 확인하기 위한 분기문
					{
						int end_time = uptime(); //
						printf(1, "PID : %d, WEIGHT : %d, ", getpid(), weightset(n+1));
						printf(1, "TIMES : %d ms\n", (end_time - start_time) * 10);
						first = 0;
					}
					print_counter = PRINT_CYCLE;
				}
				counter++;
			}
			printf(1, "PID : %d terminated\n", getpid());
			exit();
		}
	}

	//20182625 parent process는 child process를 wait해줘야 함.
	for(; n> 0; n--) // process 개수만큼 반복
	{
		if(wait() <0) // child process가 없을 시
		{
			printf(1, "wait stopped early\n");
			exit();
		}
	}

	if (wait() != -1) // child process가 모두 종료하였는데도 wait() != -1인 경우
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

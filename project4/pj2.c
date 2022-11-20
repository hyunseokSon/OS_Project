#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef struct node
{
	int data;
	struct node *next;
} Node;

typedef struct Queue
{
	Node *front; //맨 앞(꺼낼 위치)
	Node *rear;	 //맨 뒤(보관할 위치)
	int count;
} Queue;

void random_reference_string(int page_string[]); 
void cleanBuffer(void);
int menu_b(int, char fin[4]);
int menu_c(int, char fin[4]);
void do_menu_c(FILE *f1, FILE *f2, int input_data, int page_string[500]);
int check_index(int page_frame, int *pageArray, int n);
int nearIndex(int pf, int cur, int *pageArray, int n);
int checkMax(int pf, int *pageArray, int check_value);
void opt(int page_string[500], int page_frame, int *pageArray);
void fifo(int page_string[500], int page_frame, int *pageArray);
void lifo(int page_string[500], int page_frame, int *pageArray);
void initQueue(Queue *queue);
int isEmpty(Queue *queue);
int getQueue(Queue *queue, int data);
void deQueue(Queue *queue);
int searchQueue(Queue *queue, int value);
void hitQueue(Queue *queue, int value);
void lru(int page_string[500], int page_frame, Queue *queue, int *pageArray);
void lfu(int page_string[500], int page_frame, int *pageArray);
int updateSC(int data, int frame, int *pageArray, bool second_chance[frame], int pointer);
void sc(int page_string[500], int page_frame, int *pageArray);

int main()
{
	while (1)
	{
		char menu[50] = {0};		// 알고리즘 시뮬레이터 선택
		char *choiceMenu[10] = {0}; // 선택된 페이지 알고리즘
		int realMenu[10] = {0};		// 선택된 알고리즘 메뉴
		char fin[5] = "2625";		// 학번 20182625 중 뒤의 네자리 2625 입력시 프로그램 종료
		int i = 0, c;				// 조건문을 위한 변수
		int flag = 0;				// 조건을 충족하는 지 판별하기 위한 값
		int flagEight = 0;			// A. 메뉴 선택 시 8. All이 있는지 판별하기 위한 값
		int flagOpt = 0;			// 메뉴에서 Optimal 을 선택한 적이 있는지 판별하기 위한 값
		int page_string[500] = {0}; // 500개의 page string
		int page_frame = 0;			// page frame 개수
		int input_data;				// 데이터 입력 방식
		FILE *f1, *f2, *fp, *fp2;	// 파일 입력 및 출력을 위한 파일포인터
		Queue queue;				// LRU구현을 위한 QUEUE

		printf("A. Page Replacement 알고리즘 시뮬레이터를 선택하시오. (최대 3개 선택 가능, 종료 명령어 : 2625)\n");
		printf("(1) Optimal (2) FIFO (3) LIFO (4) LRU (5) LFU (6) SC (7) ESC (8) ALL\n");
		fgets(menu, 50, stdin);
		menu[strlen(menu) - 1] = 0;

		if (strcmp(menu, fin) == 0)
		{
			printf("프로그램을 종료합니다...\n");
			break;
		}

		char *ptr = strtok(menu, " "); //공백 문자열을 기준으로 문자열을 자른다.
		while (ptr != NULL)
		{

			choiceMenu[i] = ptr; // 공백 이전의 문자열을 가져온다.
			realMenu[i] = atoi(choiceMenu[i]);

			if (realMenu[i] == 8)
			{
				flagEight = 1; // ALL 선택시 flagEight = 1 로 설정.
				break;
			}

			if (realMenu[i] < 1 || realMenu[i] > 8) // 메뉴 선택 시 1~8 사이의 값이 아닐 때
			{
				printf("\n\n");
				printf("입력 에러 : 범위 사이의 값(1~8)을 입력하세요.\n");
				flag = 1;
				break;
			}

			i++;
			ptr = strtok(NULL, " ");

			// 8.all 을 제외하고 최대 3개의 알고리즘을 선택할 수 있도록 조건 설정
			if (i > 3 && flagEight == 0) 
			{
				printf("\n\n");
				printf("입력 에러 : 최대 3개의 값만 입력 가능합니다. 다시 시도하세요.\n");
				flag = 1;
				break;
			}
		}

		if (flag == 1) // 범위 안의 값을 입력하지 않았을 시 다시 메뉴 선택을 하도록 한다.
			continue;

		page_frame = menu_b(page_frame, fin); // B 메뉴 호출
		int pageArray[page_frame];			  // page frame값 만큼을 가지는 pageArray 배열 선언.

		input_data = menu_c(input_data, fin);		// C 메뉴 호출
		do_menu_c(f1, f2, input_data, page_string); // input_data에 따른 데이터 입력 방식 처리 함수

		fp = fopen("20182625.txt", "w+");
		fprintf(fp,"---------참조 스트링---------\n");

		for (c=0; c<500; c++)
			fprintf(fp, "%3d", page_string[c]);

		if (flagEight == 0) // 8번 메뉴를 선택하지 않았을 때
		{

			printf("\n\n");
			printf("선택하신 메뉴는");
			fprintf(fp,"\n\n선택하신 메뉴는 ");

			for (int c = 0; c< 10; c++)
			{
				if (realMenu[c] == 1)
					flagOpt = 1;
			}

			// 1번 메뉴를 선택했을 때
			if (flagOpt) 
			{
				for (int c = 0; c < 10; c++)
				{
					if (realMenu[c] != '\0') {
						printf("%2d번", realMenu[c]);
						fprintf(fp,"%2d번", realMenu[c]);
					}
				}
				printf("입니다!!\n");
				fprintf(fp,"입니다!!\n");
				fclose(fp);

				for (c= 0; c<10; c++)
				{
					if (realMenu[c] == 1)
					{
						// 1번 Optimal 선택 시
						opt(page_string, page_frame, pageArray);
					}
					else if (realMenu[c] == 2)
					{
						// 2번 FIFO 선택 시
						fifo(page_string, page_frame, pageArray);
					}
					else if (realMenu[c] == 3)
					{
						// 3번 LIFO 선택 시
						lifo(page_string, page_frame, pageArray);
					}
					else if (realMenu[c] == 4)
					{
						// 4번 LRU 선택 시
						lru(page_string, page_frame, &queue, pageArray);
					}
					else if (realMenu[c] == 5)
					{
						// 5번 LFU 선택 시
						lfu(page_string, page_frame, pageArray);
					}
					else if (realMenu[c] == 6)
					{
						// 6번 SC 선택 시
						sc(page_string, page_frame, pageArray);
					}
					else if (realMenu[c] == 7)
					{
						// 7번 ESC 선택 시
						printf("메뉴 7번 선택..\n");
					}
				}
			}
			
			// 1번 메뉴를 선택하지 않았을 때
			else  
			{
				for (int c = 0; c < 10; c++)
				{
					if (realMenu[c] != '\0') {
						printf("%2d번", realMenu[c]);
						fprintf(fp,"%2d번", realMenu[c]);
					}
				}
				printf("입니다!!\n");
				fprintf(fp,"입니다!!\n");
				fclose(fp);

				for (c= 0; c<10; c++)
				{
					if (realMenu[c] == 2)
					{
						// 2번 FIFO 선택 시
						fifo(page_string, page_frame, pageArray);
					}
					else if (realMenu[c] == 3)
					{
						// 3번 LIFO 선택 시
						lifo(page_string, page_frame, pageArray);
					}
					else if (realMenu[c] == 4)
					{
						// 4번 LRU 선택 시
						lru(page_string, page_frame, &queue, pageArray);
					}
					else if (realMenu[c] == 5)
					{
						// 5번 LFU 선택 시
						lfu(page_string, page_frame, pageArray);
					}
					else if (realMenu[c] == 6)
					{
						// 6번 SC 선택 시
						sc(page_string, page_frame, pageArray);
					}
					else if (realMenu[c] == 7)
					{
						// 7번 ESC 선택 시
						printf("메뉴 7번 선택..\n");
					}
				}
				// OPTIMAL과 비교를 위해 1번 메뉴를 마지막에 수행하여 결과를 비교.
				printf("\n\n 결과 비교를 위하여 Optimal 알고리즘을 수행합니다..\n\n");
				fp2 = fopen("20182625.txt", "a");
				fprintf(fp2,"\n 결과 비교를 위하여 Optimal 알고리즘을 수행합니다..\n");
				fclose(fp2);
				opt(page_string, page_frame, pageArray);
			}
		}

		else // 8번 메뉴를 선택했을 때
		{
			fprintf(fp,"모든 알고리즘을 수행합니다.\n");
			fclose(fp);
		}
	} // A메뉴에 대한 무한루프 종료.
	return 0;
}

// 1~30을 500번 돌린 후 page_string배열에 넣는다.
void random_reference_string(int page_string[500])
{
	// srand(time(NULL));
	int random = 0;
	for (int n = 0; n < 500; n++)
	{
		random = rand() % 30 + 1; // 1~30 random
		page_string[n] = random;
	}
}

// 입력 버퍼 비워주는 함수.
void cleanBuffer(void)
{
	while (getchar() != '\n')
		;
}

// B 메뉴 호출 함수.
int menu_b(int page_frame, char fin[4])
{
	while (1)
	{
		int end = 0;
		page_frame = 0;

		printf("B. 페이지 프레임의 개수를 입력하시오.(3~10) (종료 명령어 : 2625)\n");
		scanf("%d", &page_frame);
		cleanBuffer();

		end = atoi(fin);
		if (page_frame == end)
		{
			printf("프로그램을 종료합니다..\n");
			exit(1);
		}

		else if (page_frame >= 3 && page_frame <= 10)
		{
			return page_frame;
			break;
		}
		else
		{
			printf("사용자 입력 에러 : 범위(3~10)에 맞게 입력해주세요.\n");
		}
	}
}

// C 메뉴 호출 함수.
int menu_c(int input_data, char fin[4])
{
	while (1)
	{
		int end = 0;
		input_data = 0;

		printf("C. 데이터 입력 방식을 선택하시오.(1,2) (종료 명령어 : 2625)\n");
		scanf("%d", &input_data);
		cleanBuffer();

		end = atoi(fin);
		if (input_data == end)
		{
			printf("프로그램을 종료합니다..\n");
			exit(1);
		}

		else if (input_data == 1 || input_data == 2)
		{
			return input_data;
			break;
		}

		else
			printf("사용자 입력 에러 : 범위(1,2)에 맞게 입력해주세요.\n");
	}
}

// 파일에서 랜덤하게 페이지 스트링 생성 및 가져온다.
void do_menu_c(FILE *f1, FILE *f2, int input_data, int page_string[500])
{
	// 데이터 입력 방식 1일 때.
	// 즉, 랜덤하게 생성할 때

	if (input_data == 1)
	{
		random_reference_string(page_string);
		for (int j = 0; j < 500; j++)
		{
			printf("%3d", page_string[j]);
		}

		printf("\n");
	}

	// 데이터 입력 방식 2일 때.
	// 즉, 사용자 생성 파일을 오픈할 때.
	else if (input_data == 2)
	{
		int temp = 0;
		char tmp[2000] = {0};
		char *get_file_page_string[500] = {0};

		random_reference_string(page_string); // 랜덤으로 스트링 패턴 작성

		f1 = fopen("reference_string.txt", "w+");

		for (int j = 0; j < 500; j++)
		{
			fprintf(f1, "%3d", page_string[j]);
		}

		if (f1 == NULL)
			printf("파일 열기에 실패하였습니다...\n");
		else
			printf("파일을 정상적으로 만들었습니다!\n");

		fclose(f1);

		f2 = fopen("reference_string.txt", "r+");
		fread(tmp, 1, 2000, f2);

		// 파일에서 값을 가져온 후 공백을 기준으로 값을 나눠 출력해준다.
		char *pointer = strtok(tmp, " ");
		while (pointer != NULL)
		{
			get_file_page_string[temp] = pointer;
			temp++;
			pointer = strtok(NULL, " ");
		}

		for (int m = 0; m < 500; m++)
		{
			page_string[m] = atoi(get_file_page_string[m]);
			printf("%d ", page_string[m]);
		}
		printf("\n");
		fclose(f2);
	}
}

// 페이지 프레임에 참조 스트링 값이 있는지 확인하는 함수
int check_index(int page_frame, int *pageArray, int n)
{
	for (int k = 0; k < page_frame; k++)
	{
		if (pageArray[k] == n)
			return 1;
	}

	return 0;
}

// 가장 가까이 있는 것 확인
int nearIndex(int total, int cur, int *pageArray, int n)
{
	// tatal은 전체 참조 스트림 갯수, cur은 현재 위치,
	// n은 참조 스트링 값을 의미한다.
	int a = cur;
	for (a++; a < total; a++)
	{
		if (pageArray[a] == n)
		{
			return a;
		}
	}
	return a;
}

// 가장 멀리 있는 값을 확인하는 함수
int checkMax(int pf, int *pageArray, int check_value)
{
	//pf는 page_frame을 의미.
	int ch = 0;
	if (check_value == 1) // 최대일 때
	{
		int n = 0;
		int big = pageArray[0];

		for (n++; n < pf; n++)
		{
			if (pageArray[n] > big)
			{
				big = pageArray[n];
				ch = n;
			}
		}
		return ch;
	}

	else
	{
		int n = 0;
		int small = pageArray[0];
		for (n++; n < pf; n++)
		{
			if (pageArray[n] < small)
			{
				small = pageArray[n];
				ch = n;
			}
		}
		return ch;
	}
}

// 1번 메뉴 : Optimal
void opt(int page_string[500], int page_frame, int *pageArray)
{
	FILE *fp;
	int index = -1;
	int fault = 0, flag_fault = 0;
	int t;
	int block = 0;
	int nIndex[page_frame]; // 다음 인덱스를 의미하는 배열

	fp = fopen("20182625.txt", "a");
	printf("-----------------------------------------");
	printf("\n (1). OPTIMAL \n");
	printf("-----------------------------------------");
	printf("\n참조스트림");

	fprintf(fp, "-----------------------------------------");
	fprintf(fp, "\n (1). OPTIMAL \n");
	fprintf(fp, "-----------------------------------------");
	fprintf(fp, "\n참조스트림");

	//초기에 있는 페이지 값들을 모두 -1로 초기화
	for (t = 0; t < page_frame; t++)
	{
		pageArray[t] = -1;
		printf("  page_frame[%d] ", t);
		fprintf(fp,"  page_frame[%d] ", t);
	}

	printf("\t페이지 폴트\n");
	fprintf(fp,"\t페이지 폴트\n");

	for (t = 0; t < 500; t++)
	{
		// hit일 때
		if (check_index(block, pageArray, page_string[t]))
		{
			flag_fault = 0;
		}
		// fault일 때
		else
		{
			//페이지가 꽉 찼을 때
			if (block == page_frame)
			{
				int tmp = 0;
				for (; tmp < page_frame; tmp++)
				{
					nIndex[tmp] = nearIndex(500, t, page_string, pageArray[tmp]);
				}
				int goal = checkMax(page_frame, nIndex, 1);
				pageArray[goal] = page_string[t];
			}

			// 페이지에 빈 공간이 있을 때
			else
			{
				pageArray[(++index) % page_frame] = page_string[t];
				block++;
			}
			fault++;
			flag_fault = 1;
		}

		//결과 과정 출력
		printf("    %d\t", page_string[t]);
		fprintf(fp,"    %d\t", page_string[t]);

		for (int tmp = 0; tmp < page_frame; tmp++)
		{
			printf("%11d\t", pageArray[tmp]);
			fprintf(fp,"%11d\t", pageArray[tmp]);
		}

		if (flag_fault)
		{
			printf("\tPAGE FAULT\n");
			fprintf(fp,"\tPAGE FAULT\n");
			flag_fault = 0;
		}

		else
		{
			printf("\t          \n");
			fprintf(fp,"\t          \n");
			flag_fault = 0;
		}
	}

	printf("\nOptimal: \n");
	printf("\tPage faults : %d\n", fault);
	printf("\t       Hits : %d\n", 500 - fault);
	printf("\t  Hit Ratio : %.2f%%\n", (500 - fault) * 100.0 / 500);
	printf("-----------------------------------------\n\n");
	
	fprintf(fp,"\nOptimal: \n");
	fprintf(fp,"\tPage faults : %d\n", fault);
	fprintf(fp,"\t       Hits : %d\n", 500 - fault);
	fprintf(fp,"\t  Hit Ratio : %.2f%%\n", (500 - fault) * 100.0 / 500);
	fprintf(fp,"-----------------------------------------\n\n");
	fclose(fp);
}

// 2번 메뉴 : FIFO
void fifo(int page_string[500], int page_frame, int *pageArray)
{
	FILE *f;
	int index = -1;
	int fault = 0, t;
	int flag_fault = 0;

	f = fopen("20182625.txt", "a");

	printf("-----------------------------------------");
	printf("\n (2). FIFO \n");
	printf("-----------------------------------------");
	printf("\n참조스트림");
	
	fprintf(f,"-----------------------------------------");
	fprintf(f,"\n (2). FIFO \n");
	fprintf(f,"-----------------------------------------");
	fprintf(f,"\n참조스트림");

	//초기에 있는 페이지 값들을 모두 -1로 초기화
	for (t = 0; t < page_frame; t++)
	{
		pageArray[t] = -1;
		printf("  page_frame[%d] ", t);
		fprintf(f,"  page_frame[%d] ", t);
	}

	printf("\t페이지 폴트\n");
	fprintf(f,"\t페이지 폴트\n");
	for (t = 0; t < 500; t++)
	{
		// hit일 때
		if (check_index(page_frame, pageArray, page_string[t]))
			flag_fault = 0;

		// fault일 때
		else
		{
			pageArray[(++index) % page_frame] = page_string[t];
			fault++;
			flag_fault = 1;
		}

		//결과 과정 출력
		printf("    %d\t", page_string[t]);
		fprintf(f,"    %d\t", page_string[t]);
		for (int tmp = 0; tmp < page_frame; tmp++) {
			printf("%11d\t", pageArray[tmp]);
			fprintf(f,"%11d\t", pageArray[tmp]);
		}

		if (flag_fault)
		{
			printf("\tPAGE FAULT\n");
			fprintf(f,"\tPAGE FAULT\n");
			flag_fault = 0;
		}
		else
		{
			printf("\t            \n");
			fprintf(f,"\t            \n");
			flag_fault = 0;
		}
	}

	printf("\nFIFO: \n");
	printf("\tPage faults : %d\n", fault);
	printf("\t       Hits : %d\n", 500 - fault);
	printf("\t  Hit Ratio : %.2f%%\n", (500 - fault) * 100.0 / 500);
	printf("-----------------------------------------\n\n");
	
	fprintf(f,"\nFIFO: \n");
	fprintf(f,"\tPage faults : %d\n", fault);
	fprintf(f,"\t       Hits : %d\n", 500 - fault);
	fprintf(f,"\t  Hit Ratio : %.2f%%\n", (500 - fault) * 100.0 / 500);
	fprintf(f,"-----------------------------------------\n\n");
	fclose(f);
}

// 3. LIFO
void lifo(int page_string[500], int page_frame, int *pageArray)
{
	FILE *fp;
	int t;
	int index = -1;
	int fault = 0, flag_fault = 0;
	printf("-----------------------------------------");
	printf("\n (3). LIFO \n");
	printf("-----------------------------------------");
	printf("\n참조스트림");
	
	fp = fopen("20182625.txt", "a");
	fprintf(fp,"-----------------------------------------");
	fprintf(fp,"\n (3). LIFO \n");
	fprintf(fp,"-----------------------------------------");
	fprintf(fp,"\n참조스트림");

	//초기에 있는 페이지 값들을 모두 -1로 초기화
	for (t = 0; t < page_frame; t++)
	{
		pageArray[t] = -1;
		printf("  page_frame[%d] ", t);
		fprintf(fp,"  page_frame[%d] ", t);
	}

	printf("\t페이지 폴트\n");
	fprintf(fp,"\t페이지 폴트\n");

	for (t = 0; t < 500; t++)
	{
		// hit일 때
		if (check_index(page_frame, pageArray, page_string[t]))
			flag_fault = 0;

		// fault일 때
		else
		{
			index++;
			fault++;
			flag_fault = 1;
			if (index < 3) // 페이지가 비어있을 때
				pageArray[index % page_frame] = page_string[t];
			else // 페이지가 가득 찼을 때
				pageArray[page_frame - 1] = page_string[t];
		}

		//결과 과정 출력
		printf("    %d\t", page_string[t]);
		fprintf(fp,"    %d\t", page_string[t]);
		for (int tmp = 0; tmp < page_frame; tmp++) {
			printf("%11d\t", pageArray[tmp]);
			fprintf(fp,"%11d\t", pageArray[tmp]);
		}

		if (flag_fault)
		{
			printf("\tPAGE FAULT\n");
			fprintf(fp,"\tPAGE FAULT\n");
			flag_fault = 0;
		}
		else
		{
			printf("\t            \n");
			fprintf(fp,"\t            \n");
			flag_fault = 0;
		}
	}

	printf("\nLIFO: \n");
	printf("\tPage faults : %d\n", fault);
	printf("\t       Hits : %d\n", 500 - fault);
	printf("\t  Hit Ratio : %.2f%%\n", (500 - fault) * 100.0 / 500);
	printf("-----------------------------------------\n\n");
	
	fprintf(fp,"\nLIFO: \n");
	fprintf(fp,"\tPage faults : %d\n", fault);
	fprintf(fp,"\t       Hits : %d\n", 500 - fault);
	fprintf(fp,"\t  Hit Ratio : %.2f%%\n", (500 - fault) * 100.0 / 500);
	fprintf(fp,"-----------------------------------------\n\n");
	fclose(fp);
}

// 큐 초기화
void initQueue(Queue *queue)
{
	queue->front = queue->rear = NULL; // front와 rear 초기화
	queue->count = 0;				   // 보관 개수를 0으로 설정
}

// 큐가 비었는지 확인.
int isEmpty(Queue *queue)
{
	return queue->count == 0; //보관 개수가 0이면 빈 상태
}

//큐에서 새로운 node값 가져오기.
int getQueue(Queue *queue, int data)
{
	Node *now = (Node *)malloc(sizeof(Node)); //노드 생성
	now->data = data;						  //데이터 설정
	now->next = NULL;

	if (isEmpty(queue)) //큐가 비어있을 때
	{
		queue->front = now; //맨 앞을 now로 설정
	}
	else //비어있지 않을 때
	{
		queue->rear->next = now; //맨 뒤의 다음을 now로 설정
	}
	queue->rear = now; //맨 뒤를 now로 설정
	queue->count++;	   //보관 개수를 1 증가
	return now->data;
}

//큐에서 node 삭제
void deQueue(Queue *queue)
{
	Node *now;
	if (!isEmpty(queue)) 
	{
	now = queue->front;		  //맨 앞의 노드를 now에 기억
	queue->front = now->next; //맨 앞은 now의 다음 노드로 설정
	free(now);				  // now 소멸
	queue->count--;			  //보관 개수를 1 감소
	}
}

// 큐에서 검색
int searchQueue(Queue *queue, int value)
{
	Node *search;
	search = queue->front;

	while (search != NULL)
	{
		if (search->data == value)
			return 1;

		search = search->next;
	}
	return 0;
}

// LRU에서 hit 발생 시 처리 함수
void hitQueue(Queue *queue, int value)
{
	Node *n;
	n = queue->front;

	// 처음 node에 일치하는 값이 있을 때 삭제
	if (n->data == value) 
	{
		queue->front = n->next;
		free(n);
		queue->count--;
		//삭제 후 노드 추가
		getQueue(queue, value);
		return;
		//앞에 값 추가
	}

	// 중간 node에 일치하는 값이 있을 때 삭제
	else
	{
		while (n->next != NULL)
		{
			if (n->next->data == value && n->next->next != NULL)
			{
				Node *tmp = n->next;
				n->next = n->next->next;
				queue->count--;
				free(tmp); // 메모리 해제
				getQueue(queue, value);
				return;
			}
			n = n->next;
		}
	}
}

// 4. LRU
//pageArray 넣을지 말지 고민해보기.
void lru(int page_string[500], int page_frame, Queue *queue, int *pageArray)
{
	FILE *fp;
	int fault = 0, flag_fault = 0;
	int t;
	int c;

	printf("-----------------------------------------");
	printf("\n (4). LRU \n");
	printf("-----------------------------------------");
	printf("\n참조스트림");
	
	fp = fopen("20182625.txt", "a");
	fprintf(fp,"-----------------------------------------");
	fprintf(fp,"\n (4). LRU \n");
	fprintf(fp,"-----------------------------------------");
	fprintf(fp,"\n참조스트림");

	initQueue(queue); // 큐 초기화
	Node *result = (Node *)malloc(sizeof(Node));

	for (t = 0; t < page_frame; t++)
	{
		pageArray[t] = -1;
		printf("  page_frame[%d] ", t);
		fprintf(fp,"  page_frame[%d] ", t);
	}

	printf("\t페이지 폴트\n");
	fprintf(fp,"\t페이지 폴트\n");

	for (t = 0; t < 500; t++)
	{
		result = queue->front;

			// hit일 때
			if (searchQueue(queue, page_string[t]))
			{
				hitQueue(queue, page_string[t]);
				flag_fault = 0;
				result = queue->front;
				for (c = 0; c < page_frame; c++)
				{
					if (result != NULL)
					{
						pageArray[c] = result->data;
						result = result->next;
					}
				}
			}

			// fault일 때
			else
			{
				if (queue->count < page_frame)
					pageArray[t] = getQueue(queue,page_string[t]);
				deQueue(queue);
				getQueue(queue, page_string[t]);
				result = queue->front;
				fault++;
				flag_fault = 1;
				for (c = 0; c < page_frame; c++)
				{
					if (result != NULL)
					{
						pageArray[c] = result->data;
						result = result->next;
					}
				}
			}

		//결과 과정 출력
		printf("    %d\t", page_string[t]);
		fprintf(fp,"    %d\t", page_string[t]);

		// page_frame 만큼은 아래와 같이 출력
		for (c = 0; c < page_frame; c++)
		{
			printf("%11d\t", pageArray[c]);
			fprintf(fp,"%11d\t", pageArray[c]);
		}
		if (flag_fault) {
			printf("\tPAGE FAULT\n");
			fprintf(fp,"\tPAGE FAULT\n");
		}
		else {
			printf("\t          \n");
			fprintf(fp,"\t          \n");
		}

		flag_fault = 0;
	}

	printf("\nLRU: \n");
	printf("\tPage faults : %d\n", fault);
	printf("\t       Hits : %d\n", 500 - fault);
	printf("\t  Hit Ratio : %.2f%%\n", (500 - fault) * 100.0 / 500);
	printf("-----------------------------------------\n\n");
	
	fprintf(fp,"\nLRU: \n");
	fprintf(fp,"\tPage faults : %d\n", fault);
	fprintf(fp,"\t       Hits : %d\n", 500 - fault);
	fprintf(fp,"\t  Hit Ratio : %.2f%%\n", (500 - fault) * 100.0 / 500);
	fprintf(fp,"-----------------------------------------\n\n");
	fclose(fp);
}


// 5. LFU
void lfu(int page_string[500], int page_frame, int *pageArray)
{
	FILE *fp;
	int index=0;
	int counter[page_frame]; // counter를 위한 배열
	int fault = 0; // fault 발생 횟수를 세기 위한 변수
	int t,c; // 반복문을 위해 필요한 변수
	int flag_fault = 0; // hit인지 fault인지 구분을 위한 flag
	int min; // min은 최소 count를 의미
	int upIndex; // count가 같을 때 최신에 갱신됐던 자리

	printf("-----------------------------------------");
	printf("\n (5). LFU \n");
	printf("-----------------------------------------");
	printf("\n참조스트림");
	
	fp = fopen("20182625.txt", "a");
	fprintf(fp,"-----------------------------------------");
	fprintf(fp,"\n (5). LFU \n");
	fprintf(fp,"-----------------------------------------");
	fprintf(fp,"\n참조스트림");

	for (t = 0; t < page_frame; t++)
	{
		pageArray[t] = -1;
		printf("  page_frame[%d] ", t);
		fprintf(fp,"  page_frame[%d] ", t);
	}

	printf("\t페이지 폴트\n");
	fprintf(fp,"\t페이지 폴트\n");

	for (t = 0; t < 500; t++)
	{
		flag_fault = 0;

		// hit일 때
		if (check_index(page_frame, pageArray, page_string[t])) 
		{
			//찾은 값을 counter에 넣기 위해 한 번 더 search 한다.
			for (c=0; c<page_frame; c++)
			{
				if(pageArray[c] = page_string[t])
					counter[c]++;
			}
			flag_fault = 0;
		}

		// fault발생했을 때
		else
		{
			flag_fault = 1;
			if (fault < page_frame) // pageArray에 값이 다 채워지지 않았을 때
			{
				pageArray[index] = page_string[t];
				counter[index] = 1; // 값이 갱신되므로 counter는 1이 된다.
				index = (index+1) % page_frame;
				fault++;
			}

			else // pageArray에 값이 다 채워진 상태에서 fault가 발생했을 때
			{
				upIndex=0;
				min=counter[0]; 
				for(c=1; c<page_frame; c++)
				{
					if (counter[c]<min)
					{
						upIndex=c;
						min=counter[c];
					}
				}

				pageArray[upIndex] = page_string[t];
				counter[upIndex]=1;
				fault++;
			}
		}

		//결과 과정 출력
		printf("    %d\t", page_string[t]);
		fprintf(fp,"    %d\t", page_string[t]);
		for (c = 0; c < page_frame; c++) {
			printf("%11d\t", pageArray[c]);
			fprintf(fp,"%11d\t", pageArray[c]);
		}

		if (flag_fault)
		{
			printf("\tPAGE FAULT\n");
			fprintf(fp,"\tPAGE FAULT\n");
		}
		else
		{
			printf("\t            \n");
			fprintf(fp,"\t            \n");
		}
	}

	printf("\nLFU: \n");
	printf("\tPage faults : %d\n", fault);
	printf("\t       Hits : %d\n", 500 - fault);
	printf("\t  Hit Ratio : %.2f%%\n", (500 - fault) * 100.0 / 500);
	printf("-----------------------------------------\n\n");
	
	fprintf(fp,"\nLFU: \n");
	fprintf(fp,"\tPage faults : %d\n", fault);
	fprintf(fp,"\t       Hits : %d\n", 500 - fault);
	fprintf(fp,"\t  Hit Ratio : %.2f%%\n", (500 - fault) * 100.0 / 500);
	fprintf(fp,"-----------------------------------------\n\n");
	fclose(fp);
}

// second chance 알고리즘
int updateSC(int data, int frame, int *pageArray, bool second_chance[frame], int pointer)
{
	while(true)
	{
		if(!second_chance[pointer])
		{
			pageArray[pointer] = data;
			return (pointer +1) % frame;
		}
		second_chance[pointer] = false;
		pointer = (pointer+1) % frame;
	}
}

void sc(int page_string[500], int page_frame, int *pageArray)
{
	FILE *fp;
	int t,c;
	int pointer = 0;
	int flag_hit, fault=0;
	bool second_chance[page_frame];

	printf("-----------------------------------------");
	printf("\n (6). SC \n");
	printf("-----------------------------------------");
	printf("\n참조스트림");
	
	fp = fopen("20182625.txt", "a");
	fprintf(fp,"-----------------------------------------");
	fprintf(fp,"\n (6). SC \n");
	fprintf(fp,"-----------------------------------------");
	fprintf(fp,"\n참조스트림");

	for(t=0; t< page_frame; t++)
	{
		pageArray[t] = -1;
		printf("  page_frame[%d] ", t); 
		fprintf(fp,"  page_frame[%d] ", t); 
	}

	printf("\t페이지 폴트\n");
	fprintf(fp,"\t페이지 폴트\n");

	for (t=0; t<500; t++)
	{
		flag_hit = 0;
		for (c=0; c<page_frame; c++)
		{
			if(pageArray[c] == page_string[t])
			{
				second_chance[c] = true;
				//hit이면 check=1
				flag_hit = 1;
			}

		}

		//fault일 때
		if (flag_hit == 0)
		{
			pointer = updateSC(page_string[t], page_frame, pageArray, second_chance, pointer);
			fault++;
		}

		//결과 과정 출력
        printf("    %d\t", page_string[t]);
        fprintf(fp,"    %d\t", page_string[t]);

		for (c = 0; c < page_frame; c++)
        {
            printf("%11d\t", pageArray[c]);
            fprintf(fp,"%11d\t", pageArray[c]);
        }

		if (flag_hit==0) {
            printf("\tPAGE FAULT\n");
            fprintf(fp,"\tPAGE FAULT\n");
        }
        else {
            printf("\t          \n");
            fprintf(fp,"\t          \n");
        }
	}

	printf("\nSecond Chance: \n");
    printf("\tPage faults : %d\n", fault);
    printf("\t       Hits : %d\n", 500 - fault);
    printf("\t  Hit Ratio : %.2f%%\n", (500 - fault) * 100.0 / 500);
    printf("-----------------------------------------\n\n");

    fprintf(fp,"\nSecond Chance: \n");
    fprintf(fp,"\tPage faults : %d\n", fault);
    fprintf(fp,"\t       Hits : %d\n", 500 - fault);
    fprintf(fp,"\t  Hit Ratio : %.2f%%\n", (500 - fault) * 100.0 / 500);
    fprintf(fp,"-----------------------------------------\n\n");
    fclose(fp);
}















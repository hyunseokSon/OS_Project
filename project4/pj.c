#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef struct node
{
	int data; // 노드에 들어갈 데이터
	struct node *next; // 다음 노드를 가리킴
} Node;

typedef struct Queue
{
	Node *front; //맨 앞(꺼낼 위치)
	Node *rear;	 //맨 뒤(보관할 위치)
	int count; // 노드 개수
} Queue;

void random_reference_string(int page_string[]); //무작위로 참조 스트림 만드는 함수
void cleanBuffer(void); // 입력 버퍼 비워주는 함수
int menu_b(int, char fin[4]); // B 메뉴 호출 함수
int menu_c(int, char fin[4]); // C 메뉴 호출 함수
void do_menu_c(FILE *f1, FILE *f2, int input_data, int page_string[500]); // C의 데이터 입력 방식에 따른 처리 함수
int check_index(int page_frame, int *pageArray, int n); // 참조 스트림 값이 페이지에 있는지 확인하는 함수
int nearIndex(int pf, int cur, int *array, int n); //가장 가까이 있는 값 확인
int checkMax(int pf, int *pageArray); // 가장 멀리 있는 값 확인
void opt(int page_string[500], int page_frame, int *pageArray); // opt
void fifo(int page_string[500], int page_frame, int *pageArray); // fifo
void lifo(int page_string[500], int page_frame, int *pageArray); // lifo
void initQueue(Queue *queue); // LRU - Queue 초기화하는 함수
int isEmpty(Queue *queue); // LRU - Queue가 비어있는지 확인
int getQueue(Queue *queue, int data); // LRU - 큐에 노드 추가.
void deQueue(Queue *queue); // LRU - 큐에 노드 삭제
int searchQueue(Queue *queue, int value); // LRU - 큐에 해당 값을 가진 노드가 있는지 탐색
void hitQueue(Queue *queue, int value); // LRU - hit시 처리 함수
void lru(int page_string[500], int page_frame, Queue *queue, int *pageArray); // lru
void lfu(int page_string[500], int page_frame, int *pageArray); // lfu
void sc(int page_string[500], int page_frame, int *pageArray); // sc
void ref_str_bit(int page_string[500], char r_s_b[500][7]);
void esc(int page_string[500], int page_frame, int *pageArray, char r_s_b[500][7]); // esc

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
		char refer_stringbit[500][7] = {0,}; // ESC - 참조비트가 적힌 참조 스트링 값이 저장됨.
		int page_frame = 0;			// page frame 개수
		int input_data;				// 데이터 입력 방식
		FILE *f1, *f2, *fp, *fp2;	// 파일 입력 및 출력을 위한 파일포인터
		Queue queue;				// LRU구현을 위한 QUEUE

		printf("A. Page Replacement 알고리즘 시뮬레이터를 선택하시오. (최대 3개 선택 가능, 종료 명령어 : 2625)\n");
		printf("(1) Optimal (2) FIFO (3) LIFO (4) LRU (5) LFU (6) SC (7) ESC (8) ALL\n");
		fgets(menu, 50, stdin); // 문자열을 입력받는다.
		menu[strlen(menu) - 1] = 0; 

		// 학번(20182625)의 뒷자리인 2625를 입력하면 프로그램이 종료된다.
		if (strcmp(menu, fin) == 0)
		{
			printf("프로그램을 종료합니다...\n");
			break;
		}

		//공백 문자열을 기준으로 문자열을 자른다.
		char *ptr = strtok(menu, " ");
		while (ptr != NULL)
		{
			choiceMenu[i] = ptr; // 공백 이전의 문자열을 가져온다.
			realMenu[i] = atoi(choiceMenu[i]);
			if (realMenu[i] <1 || realMenu[i] > 8) // 메뉴가 1~8 범위로 입력되지 않았을 때
			{
				printf("\n입력 에러 : 범위 사이의 값(1~8)을 입력하세요.\n");
				flag=1;
				break;
			}
			else if(realMenu[i] == 8) { // 8을 입력하면 flagEight=1로 설정한다.
				flagEight=1;
				break;
			}
			i++;
			//3개 이상 메뉴를 선택했을 때
			if ( i > 3)
			{
				printf("\n입력 에러 : 최대 3개의 값만 입력 가능합니다. 다시 시도하세요.\n");
				flag=1;
				break;
			}
			ptr = strtok(NULL, " ");
		}

		if (flag == 1) // 범위 안의 값을 입력하지 않았을 시 다시 메뉴 선택을 하도록 한다.
			continue;

		page_frame = menu_b(page_frame, fin); // B 메뉴 호출
		int pageArray[page_frame];			  // page frame값 만큼을 가지는 pageArray 배열 선언.

		input_data = menu_c(input_data, fin);		// C 메뉴 호출
		do_menu_c(f1, f2, input_data, page_string); //데이터 입력 방식 처리 함수
		ref_str_bit(page_string,refer_stringbit); 	// ESC 구현을 위해 참조 스트림 뒤에 문자붙임

		//결과를 파일에 저장하기 위해 파일 생성.
		fp = fopen("20182625.txt", "w+");
		fprintf(fp,"---------참조 스트링---------\n");

		//참조 스트림 값 파일에 출력.
		for (c=0; c<500; c++)
			fprintf(fp, "%3d", page_string[c]);

		// 8번 메뉴를 선택하지 않았을 때
		if (flagEight == 0) 
		{
			printf("\n\n선택하신 메뉴는");
			fprintf(fp,"\n\n선택하신 메뉴는 ");

			//optimal 알고리즘 선택 여부
			for (int c = 0; c< 10; c++)
			{
				if (realMenu[c] == 1)
					//optimal 알고리즘을 선택했을 시
					flagOpt = 1;
			}

			// 1번 메뉴를 선택했을 때
			if (flagOpt) 
			{
				//선택한 메뉴들을 보여주는 문장
				for (c = 0; c < 10; c++)
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
						esc(page_string, page_frame, pageArray, refer_stringbit);
					}
				}
			}
			
			// 1번 메뉴를 선택하지 않았을 때
			else  
			{
				for (c = 0; c < 10; c++)
				{
					//선택한 메뉴들을 보여주는 문장
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
						esc(page_string, page_frame, pageArray, refer_stringbit);
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
			printf("\n 모든 알고리즘을 수행합니다.\n");
			fprintf(fp,"\n\n모든 알고리즘을 수행합니다.\n");
			fclose(fp); // 알고리즘 시작 전 fclose()수행 후 각 알고리즘 별로 수행 시 파일 끝에 추가.

			opt(page_string, page_frame, pageArray);
			fifo(page_string, page_frame, pageArray);
			lifo(page_string, page_frame, pageArray);
			lru(page_string, page_frame, &queue, pageArray);
			lfu(page_string, page_frame, pageArray);
			sc(page_string, page_frame, pageArray);
			esc(page_string, page_frame, pageArray, refer_stringbit);
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

		//2625 입력 시 프로그램 종료.
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
	// 즉, 랜덤하게 생성 후 화면에 출력만 해줄 때
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
	// 즉, 파일 생성 후 오픈할 때.
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
int nearIndex(int total, int cur, int *array, int n)
{
	// tatal은 전체 참조 스트림 갯수, cur은 현재 위치,
	// array는 참조 배열, n은 페이지에 있는 값을 의미한다.
	int a = cur;
	//해당 시점부터 500개의 참조 스트링까지 반복
	for (a++; a < total; a++)
	{
		if (array[a] == n)
		{
			return a;
		}
	}
	return a;
}

// 가장 멀리 있는 값을 확인하는 함수
int checkMax(int pf, int *pageArray)
{
	//pf는 page_frame을 의미.
	int ch = 0;
	int n = 0;
	int big = pageArray[0];

	//페이지 프레임 수 만큼 반복
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

// 1번 메뉴 : Optimal
void opt(int page_string[500], int page_frame, int *pageArray)
{
	FILE *fp;
	int index = -1;			// 페이지 배열 인덱스
	int fault = 0, flag_fault = 0;	//fault : fault일 때 개수, flag_fault : fault일 때의 flag값
	int t;					// 반복문을 위한 변수
	int block = 0;			// 페이지 프레임 개수만큼 반복을 수행하기 위한 변수
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
					//가장 가까이 있는 참조 스트림 값을 넣는다.
					nIndex[tmp] = nearIndex(500, t, page_string, pageArray[tmp]);
				}
				//가장 멀리 있는 값
				int goal = checkMax(page_frame, nIndex);
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

		//페이지 배열 출력
		for (int tmp = 0; tmp < page_frame; tmp++)
		{
			printf("%11d\t", pageArray[tmp]);
			fprintf(fp,"%11d\t", pageArray[tmp]);
		}

		//flag일 때
		if (flag_fault)
		{
			printf("\tPAGE FAULT\n");
			fprintf(fp,"\tPAGE FAULT\n");
			flag_fault = 0;
		}

		//hit일 때
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
	int index = -1; // 페이지 배열 인덱스
	int fault = 0, t; // fault는 fault 발생 횟수 count, t는 반복문을 위한 변수
	int flag_fault = 0; // fault인지 hit인지 구분해주는 flag

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
	int t;							//반복문을 위한 변수
	int index = -1;					//페이지 배열 인덱스
	int fault = 0, flag_fault = 0;	// fault는 총 fault 수 count, 
									// flag_fault는 fault/hit 구분 flag
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
	queue->front = queue->rear = NULL; // front와 rear값 NULL로 초기화
	queue->count = 0;				   // 보관 개수 0으로 초기화
}

// 큐가 비었는지 확인.
int isEmpty(Queue *queue)
{
	return queue->count == 0; //보관 개수가 0이면 빈 상태를 의미
}

//큐에서 새로운 node값 가져오기.
int getQueue(Queue *queue, int data)
{
	Node *now = (Node *)malloc(sizeof(Node)); // 노드를 생성
	now->data = data;						  // 데이터 지정
	now->next = NULL;

	if (isEmpty(queue)) //큐에 노드가 없을 때(비어있을 때)
	{
		queue->front = now; //맨 앞이 now가 됨.
	}
	else //노드가 있을 때(비어있지 않을 때)
	{
		queue->rear->next = now; //맨 마지막의 다음을 now로 설정
	}
	queue->rear = now; //맨 뒤를 now로 지정
	queue->count++;	   //보관 개수 1 증가
	return now->data;
}

//큐에서 node 삭제
void deQueue(Queue *queue)
{
	Node *now;
	if (!isEmpty(queue)) 	  // 큐가 비어있지 않을 때
	{
	now = queue->front;		  //맨 앞의 노드를 now로 지정
	queue->front = now->next; //맨 앞은 now의 다음 노드로 지정
	free(now);				  // now 메모리 해제
	queue->count--;			  //보관 개수 1 감소
	}
}

// 큐에서 검색
int searchQueue(Queue *queue, int value)
{
	Node *search;
	search = queue->front;	//Queue의 맨 앞에서 시작

	while (search != NULL)
	{
		if (search->data == value)	//해당 값을 찾았을 때
			return 1;

		search = search->next;
	}
	return 0;				//해당 값을 찾지 못했을 때
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
	int fault = 0, flag_fault = 0; //fault는 총 발생 횟수 count, flag_fault는 fault/hit 구분 flag
	int t,c; // 반복문을 위한 변수

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

	// 페이지 배열 값을 -1로 초기화
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
				//data를 페이지 배열에 써주고 노드를 다음 곳으로 가리킴
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
			//페이지 프레임 만큼 노드가 만들어지지 않았을 때
			if (queue->count < page_frame) 
				pageArray[t] = getQueue(queue,page_string[t]);

			else
			{
				deQueue(queue);
				getQueue(queue, page_string[t]);
				result = queue->front; //dequeue에서 front가 바뀐다.
				for (c = 0; c < page_frame; c++)
				{
					if (result != NULL)
					{
						pageArray[c] = result->data;
						result = result->next;
					}
				}
			}
			fault++;
			flag_fault = 1;
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
				if(pageArray[c] == page_string[t])
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
					//페이지 배열을 돌면서 counter값을 비교
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

//Second Chance 알고리즘
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
				//hit이면 flag_hit=1, 참조 비트 true로 변경.
				second_chance[c] = true;
				flag_hit = 1;
			}
		}

		//fault일 때
		if (flag_hit == 0)
		{
			while(true)
			{
				//대체할 page를 찾았다면
				if(!second_chance[pointer])
				{
					//페이지 배열에 값을 써준다.
					pageArray[pointer] = page_string[t];
					//포인터는 다음 노드를 가리킨다.
					pointer = (pointer+1) % page_frame;
					break;
				}
				//순환하며 돌면서 포인터와 second_chance 비트를 변경해준다.
				second_chance[pointer] = false;
				//다시 접근되지 않는 한 다음에 교체될 것이므로 포인터 변경.
				pointer = (pointer+1) % page_frame;
			}
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

void ref_str_bit(int page_string[500], char r_s_b[500][7])
{
	int i;
	char d[2] = {0};

	for(i=0; i<500; i++)
	{
		// 참조 스트링의 숫자를 저장
		sprintf(r_s_b[i], "%d", page_string[i]);
		strcat(r_s_b[i], "(");
		int t= rand() % 2;
		if (t%2 ==0)
			d[0] = 'R';
		else
			d[0] = 'W';
		strcat(r_s_b[i], d);
		strcat(r_s_b[i], ")");
	}
}

// Enhanced Second chance
void esc(int page_string[500], int page_frame, int *pageArray, char r_s_b[500][7]) // esc
{
	FILE *fp;				
	int t,c;				// 반복문을 위한 변수
	int flag_hit, fault=0;  // 
	int flag_write;			// W비트라면 1, 아니면 0
	int count;				// page_frame만큼 비교를 위한 변수.
	int check_ref;			// refernce bit값 확인을 위한 변수.
	int check_one;			// 1단계 검사(0,0)을 위한 변수.
	int pointer=0, replace=0; // 포인터 및 페이지 폴트 시 대체되는 페이지 인덱스 변수
	char *ret; // R인지 W인지 값 저장을 위한 배열
	bool ref[page_frame]; // reference bit 값 저장하는 배열
	bool mod[page_frame]; // modify bit 값 저장하는 배열

	printf("-----------------------------------------");
	printf("\n (7). ESC \n");
	printf("-----------------------------------------");
	printf("\n참조스트림");
	
	fp = fopen("20182625.txt", "a");
	fprintf(fp,"-----------------------------------------");
	fprintf(fp,"\n (7). ESC \n");
	fprintf(fp,"-----------------------------------------");
	fprintf(fp,"\n참조스트림");

	for(t=0; t< page_frame; t++)
	{
		pageArray[t] = -1;
		printf("  page_frame[%d] ", t); 
		fprintf(fp,"  page_frame[%d] ", t); 
		ref[t] = false; // 초기값 false로 세팅
		mod[t] = false; // 초기값 false로 세팅
	}

	printf("\t페이지 폴트\n");
	fprintf(fp,"\t페이지 폴트\n");

	for (t=0; t<500; t++)
	{
		flag_hit = 0;
		flag_write = 0;
		
		ret = strchr(r_s_b[t], 'W');
		if (ret != NULL) // Write bit일 때
		{
			flag_write =1;
		}
		
		for (c=0; c<page_frame; c++)
		{
			//hit라면
			if(pageArray[c] == page_string[t])
			{	
				// 참조비트 true로 설정.
				ref[c] = true;
				flag_hit = 1;
				// write bit라면
				if (flag_write)
				{
					mod[c] = true;
				}
				// read bit라면
				else
				{
					mod[c] = false;
				}
			}
		}

		//fault라면
		if (!flag_hit)
		{
			check_ref = 1;
			// reference bit가 false(0)인 값이 있는지 확인.
			for (c=0; c<page_frame; c++)
			{
				if(ref[c] == false)
				{
					check_ref = 0;
					break;
				}
			}

			// 페이지 배열의 reference bit가 모두 1이라면 0으로 바꿔준다.
			if (check_ref)
			{
				for (c=0; c<page_frame; c++)
					ref[c] = false;
			}

			// 1단계. reference bit와 modify bit가 (0,0)이 있는지를 탐색.
			check_one = 0;
			for (count = 0; count < page_frame; count++)
			{
				//포인터를 지정해준다.
				pointer = (replace+count) % page_frame;
				//(0,0)인 비트를 찾는다.
				if ((ref[pointer] == false) && (mod[pointer]==false))
					break;
			}

			// for문에서 page_frame만큼 찾았는데 break 되지 않았을 때
			if (count == page_frame)
			{
				check_one = 1;
			}

			// 2단계. (0,1) 비트를 찾는다.
			if (check_one)
			{
				for (c=0; c<page_frame; c++)
				{
					pointer = (replace+c) % page_frame;
					// (0,1) 비트를 찾았다면
					if ((ref[pointer] == false) && (mod[pointer] == true))
						break;
					//else
					//	ref[pointer]=false;
				}
			}

			fault++;
			replace = pointer;
			ref[replace] = true; // 페이지 폴트 되므로 참조 비트 1
			pageArray[replace] = page_string[t];
			// write bit라면
			if (flag_write)
			{
				mod[replace] = true;
			}
			// read bit라면
			else
			{
				mod[replace] = false;
			}
			replace++;
		}

		//결과 과정 출력
		printf("  %s\t", r_s_b[t]);
		fprintf(fp,"  %s\t", r_s_b[t]);
		
		for (c=0; c<page_frame; c++)
		{
			printf("%9d(%d,%d)\t", pageArray[c], ref[c], mod[c]);
			fprintf(fp,"%9d(%d,%d)\t", pageArray[c], ref[c], mod[c]);
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

	printf("\nEnhanced Second Chance: \n");
    printf("\tPage faults : %d\n", fault);
    printf("\t       Hits : %d\n", 500 - fault);
    printf("\t  Hit Ratio : %.2f%%\n", (500 - fault) * 100.0 / 500);
    printf("-----------------------------------------\n\n");

    fprintf(fp,"\nEnhanced Second Chance: \n");
    fprintf(fp,"\tPage faults : %d\n", fault);
    fprintf(fp,"\t       Hits : %d\n", 500 - fault);
    fprintf(fp,"\t  Hit Ratio : %.2f%%\n", (500 - fault) * 100.0 / 500);
    fprintf(fp,"-----------------------------------------\n\n");
	fclose(fp);
}













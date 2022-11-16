#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void delete_space(char s[]);
void space_split(char menu[10], char *choiceMenu[10]);
void random_reference_string(int page_string[]);
void cleanBuffer(void);
int menu_b(int, char fin[4]);
int menu_c(int, char fin[4]);
void do_menu_c(FILE *f1, FILE *f2, int input_data, int page_string[500]);
int check_index(int page_frame, int *pageArray, int n);
int nearIndex(int pf, int cur, int *pageArray, int n, int d);
int checkMax(int pf, int *pageArray, int check_value);
void opt(int page_string[500], int page_frame, int *pageArray);
void fifo(int page_string[500], int page_frame, int *pageArray);

int main()
{
	while(1)
	{
		char menu[10] = {0}; // 알고리즘 시뮬레이터 선택
		char *choiceMenu[10] = {0}; // 선택된 페이지 알고리즘
		int realMenu[10]={0};		// 선택된 알고리즘 메뉴
		char fin[5]="2625";			// 20182625 입력시 프로그램 종료.
		int i=0;
		int flag=0;					// 조건을 충족하는 지 판별하기 위한 값.
		int flagEight=0;			// A. 메뉴 선택 시 8. All이 있는지 판별하기 위한 값.
		int page_string[500] = {0};	// 500개의 page string.
		int page_frame=0;			// page frame 개수
		int input_data;				// 데이터 입력 방식
		FILE *f1,*f2;				// 파일 입력 및 출력을 위한 파일포인터

		printf("A. Page Replacement 알고리즘 시뮬레이터를 선택하시오. (최대 3개 선택 가능, 종료 명령어 : 2625)\n");
		printf("(1) Optimal (2) FIFO (3) LIFO (4) LRU (5) LFU (6) SC (7) ESC (8) ALL\n");
		fgets(menu, 10, stdin);
		menu[strlen(menu)-1]=0;

		if(strcmp(menu, fin)==0) 
		{
			printf("프로그램을 종료합니다...\n");
			break;
		}

		char *ptr = strtok(menu, " "); //공백 문자열을 기준으로 문자열을 자른다.
		while(ptr != NULL)
		{

			choiceMenu[i] = ptr; // 공백 이전의 문자열을 가져온다.
			realMenu[i] = atoi(choiceMenu[i]);

			if (realMenu[i] == 8)
			{
				flagEight = 1;
				break;
			}

			if (realMenu[i] < 1 || realMenu[i] > 8) {
				printf("\n\n");
				printf("입력 에러 : 범위 사이의 값(1~8)을 입력하세요.\n");
				flag=1;
				break;
			}

			i++;
			ptr = strtok(NULL, " ");
			if (i > 3 && flagEight ==0) // 8.all 을 제외하고 최대 3개의 알고리즘을 선택할 수 있도록 조건 설정
			{
				printf("\n\n");
				printf("입력 에러 : 최대 3개의 값만 입력 가능합니다. 다시 시도하세요.\n");
				flag=1;
				break;
			}
		}
		
		if (flag==1) // 범위 안의 값을 입력하지 않았을 시 다시 메뉴 선택을 하도록 한다.
			continue;

		page_frame = menu_b(page_frame, fin); // B 메뉴 호출
		int pageArray[page_frame]; // page frame

		input_data = menu_c(input_data, fin); // C 메뉴 호출
		do_menu_c(f1, f2, input_data, page_string); // input_data에 따른 데이터 입력 방식 처리 함수 

		if (flagEight ==0) // 8번 메뉴를 선택하지 않았을 때
		{
			printf("\n\n");
			printf("선택하신 메뉴는");
			for (int i=0; i<10; i++)
			{
				if (realMenu[i] != '\0')
					printf("%2d번", realMenu[i]);
			}
			printf("입니다!!\n");

			for (int i=0; i<10; i++)
			{
				if (realMenu[i] == 1)
				{
					// 1번 Optimal 선택 시
					opt(page_string, page_frame, pageArray);
				}
				else if (realMenu[i] == 2)
				{
					// 2번 FIFO 선택 시
					fifo(page_string, page_frame, pageArray);
				}
				else if (realMenu[i] == 3)
				{
					printf("메뉴 3번 선택..\n");
				}
				else if (realMenu[i] == 4)
				{
					printf("메뉴 4번 선택..\n");
				}
				else if (realMenu[i] == 5)
				{
					printf("메뉴 5번 선택..\n");
				}
				else if (realMenu[i] == 6)
				{
					printf("메뉴 6번 선택..\n");
				}
				else if (realMenu[i] == 7)
				{
					printf("메뉴 7번 선택..\n");
				}

			}
		}

		else			  // 8번 메뉴를 선택했을 때
		{
			printf("축하합니다!! ALL 선택!!\n");
		}

	}// A메뉴에 대한 무한루프 종료.
	return 0;
}

// A. 메뉴에 대한 선택을 할 때 공백을 제거해주는 함수.
void delete_space(char deleteSpaceMenu[10])
{
	char check[10]={0};
	int t,k=0;

	for(t=0; t<strlen(deleteSpaceMenu); t++)
	{
		if(deleteSpaceMenu[t] != ' ')
			check[k++]=deleteSpaceMenu[t];
	}
	check[k]=0;
	strcpy(deleteSpaceMenu, check);
}


//1~30을 500번 돌린 후 page_string배열에 넣는다.
void random_reference_string(int page_string[500])
{
	//srand(time(NULL));
	int random = 0;
	for (int n=0; n<500; n++)
	{
		random = rand() % 30 + 1; // 1~30 random
		page_string[n] = random;
	}
}

// 입력 버퍼 비워주는 함수.
void cleanBuffer(void)
{
	while(getchar() != '\n');
}

// B 메뉴 호출 함수.
int menu_b(int page_frame, char fin[4])
{
	while(1)
	{
		int end=0;
		page_frame=0;

		printf("B. 페이지 프레임의 개수를 입력하시오.(3~10) (종료 명령어 : 2625)\n");
		scanf("%d", &page_frame);
		cleanBuffer();

		end = atoi(fin);
		if (page_frame == end)
		{
			printf("프로그램을 종료합니다..\n");
			exit(1);
		}

		else if (page_frame>=3 && page_frame <= 10) {
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
	while(1)
	{
		int end=0;
		input_data=0;

		printf("C. 데이터 입력 방식을 선택하시오.(1,2) (종료 명령어 : 2625)\n");
		scanf("%d", &input_data);
		cleanBuffer();

		end = atoi(fin);
		if (input_data == end)
		{
			printf("프로그램을 종료합니다..\n");
			exit(1);
		}

		else if (input_data ==1 || input_data ==2)
		{
			return input_data;
			break;
		}

		else
			printf("사용자 입력 에러 : 범위(1,2)에 맞게 입력해주세요.\n");
	}
}

// 파일에서 랜덤하게 페이지 스트링 생성 및 가져옴.
void do_menu_c(FILE *f1, FILE *f2, int input_data, int page_string[500]) {
	// 데이터 입력 방식 1일 때.
	// 즉, 랜덤하게 생성할 때

	if(input_data==1)
	{
		random_reference_string(page_string);
		for (int j=0; j<500; j++)
		{
			printf("%3d", page_string[j]);
		}

		printf("\n");
		// 후에 처리할 것 구현해야 함!!!!!!
	}

	// 데이터 입력 방식 2일 때.
	// 즉, 사용자 생성 파일을 오픈할 때.
	else if (input_data==2)
	{
		int temp = 0;
		char tmp[2000] = {0};
		char *get_file_page_string[500] = {0};

		random_reference_string(page_string); // 랜덤으로 스트링 패턴 작성

		f1 = fopen("refer_str_file.txt", "w+");

		for(int j=0; j<500; j++) {
			fprintf(f1, "%3d", page_string[j]);
		}

		if (f1 == NULL)
			printf("파일 열기에 실패하였습니다...\n");
		else 
			printf("파일을 정상적으로 만들었습니다!\n");

		fclose(f1);

		f2 = fopen("refer_str_file.txt", "r+");
		fread(tmp,1,2000,f2);

		char *pointer = strtok(tmp, " ");
		while (pointer != NULL)
		{
			get_file_page_string[temp]=pointer;
			temp++;
			pointer = strtok(NULL, " ");
		}

		for (int m=0; m<500; m++) 
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
	for(int k=0; k< page_frame ; k++)
	{
		if(pageArray[k] == n)
			return 1;
	}

	return 0;
}

// 가장 가까이 있는 것 확인 (체크해볼 것)
int nearIndex(int pf, int cur, int *pageArray, int n, int d)
{
	//pf 는 page frame, cur은 현재 위치, n은 참조 스트링 값, q는 ??? (확인)
	int a = cur;
	if (d == -1) 
	{
		for(a--; a>=0; a--) 
		{
			if (pageArray[a] == n)
				return a;
		}
		return a;
	}
	else 
	{
		for(a++; a>pf; a++)
		{
			if (pageArray[a] == n)
			{
				return a;
			}
		}
		return a;
	}
}

int checkMax(int pf, int *pageArray, int check_value)
{
	int ch = 0;
	if (check_value==1) // 최대일 때
	{
		int n=0;
		int big = pageArray[0];

		for (n++; n<pf; n++)
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
		int n=0;
		int small = pageArray[0];
		for(n++; n < pf; n++) {
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
	int index = -1;
	int fault = 0, flag_fault = 0;
	int t;
	int block = 0;
	int nIndex[page_frame];

	printf("page_frame : %d\n", page_frame);
	printf("-----------------------------------------");
	printf("\n (2). FIFO \n");
	printf("-----------------------------------------");
	printf("\n참조스트림");

	for(t=0; t < page_frame; t++) 
	{
		pageArray[t] = -1;
		printf("  page_frame[%d] ", t);
	}

	printf(" 페이지 폴트\n");

	for (t=0; t<500; t++)
	{
		if(check_index(block, pageArray, page_string[t]))
		{
			flag_fault = 0;
		}
		else
		{
			if(block == page_frame) 
			{
				int tmp=0;
				for (; tmp < page_frame; tmp++)
				{
					nIndex[tmp] = nearIndex(500, t, page_string, pageArray[tmp], 1);
				}
				int goal = checkMax(page_frame, nIndex, 1);
				pageArray[goal] = page_string[t];
			}

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
		
		for(int tmp=0; tmp < page_frame ; tmp++)
		{
			printf("%11d\t", pageArray[tmp]);
		}

		if(flag_fault) {
			printf("\t   PAGE FAULT\n");
			flag_fault=0;
		}

		else {
			printf("\t            \n");
			flag_fault=0;
		}
	}

	printf("\nOptimal: \n");
    printf("\tPage faults : %d\n", fault);
    printf("\t       Hits : %d\n", 500 - fault);
    printf("\t  Hit Ratio : %.2f%%\n", (500-fault)*100.0/500);

}

// 2번 메뉴 : FIFO
void fifo(int page_string[500], int page_frame, int *pageArray)
{
	int index = -1;
	int fault = 0, t;
	int flag_fault=0;

	printf("page_frame : %d\n", page_frame);
	printf("-----------------------------------------");
	printf("\n (2). FIFO \n");
	printf("-----------------------------------------");
	printf("\n참조스트림");

	for(t=0; t < page_frame; t++) 
	{
		pageArray[t] = -1;
		printf("  page_frame[%d] ", t);
	}

	printf(" 페이지 폴트\n");
	for(t=0; t < 500; t++)
	{
		// hit일 때
		if(check_index(page_frame, pageArray, page_string[t])) 
			flag_fault=0;

		//fault발생했을 때
		else
		{
			pageArray[(++index) % page_frame] = page_string[t];
			fault++;
			flag_fault=1;
		}

		//결과 과정 출력
		printf("    %d\t", page_string[t]);
		for(int tmp=0; tmp < page_frame ; tmp++)
			printf("%11d\t", pageArray[tmp]);

		if(flag_fault) {
			printf("\t   PAGE FAULT\n");
			flag_fault=0;
		}
		else {
			printf("\t            \n");
			flag_fault=0;
		}
	}

	printf("\nOptimal: \n");
    printf("\tPage faults : %d\n", fault);
    printf("\t       Hits : %d\n", 500 - fault);
    printf("\t  Hit Ratio : %.2f%%\n", (500-fault)*100.0/500);
}

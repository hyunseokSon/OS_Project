#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void delete_space(char s[]);
void space_split(char menu[10], char *choiceMenu[10]);
void random_reference_string(int page_string[]);
void cleanBuffer(void);
void menu_b(int, char fin[4]);
int menu_c(int, char fin[4]);
void do_menu_c(FILE *f1, FILE *f2, int input_data, int page_string[500], char *get_file_page_string[500]);


int main()
{
	while(1)
	{
		char menu[10] = {0}; // 알고리즘 시뮬레이터 선택
		char *choiceMenu[10] = {0}; // 선택된 페이지 알고리즘
		char deleteSpaceMenu[10] = {0}; // 공백 제거 시 문자가 있는지 확인을 위한 배열
		char fin[5]="2625";			// 20182625 입력시 프로그램 종료.
		int i=0;
		int flag=0;					// 조건을 충족하는 지 판별하기 위한 값.
		int flagEight=0;			// A. 메뉴 선택 시 8. All이 있는지 판별하기 위한 값.
		int page_string[500] = {0};	// 500개의 page string.
		char *get_file_page_string[500]={0}; // 파일에서 받아올 page string.
		int page_frame;				// page frame 개수
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

		strcpy(deleteSpaceMenu, menu); // deleteSpaceMenu에 menu를 복사.
		delete_space(deleteSpaceMenu); // 1~8 사이의 숫자 이외의 값이 들어 있는지 확인하기 위해 공백 제거.

		for (int j=0; j < strlen(deleteSpaceMenu); j++)
		{
			if (deleteSpaceMenu[j] == '8')
				flagEight=1;			// 메뉴 선택 시 8.All 을 선택했다면 flagEight=1.

			else if (deleteSpaceMenu[j] >= '1' && deleteSpaceMenu[j] <= '8')
				continue;
			
			else
			{
				printf("사용자 입력 에러 : 범위 안의 값(1~8)을 입력하세요.\n");
				flag=1;
				break;
			}
		}

		char *ptr = strtok(menu, " "); //공백 문자열을 기준으로 문자열을 자른다.
		while(ptr != NULL)
		{

			choiceMenu[i] = ptr; // 공백 이전의 문자열을 가져온다.
			i++;
			ptr = strtok(NULL, " ");
			if (i > 3 && flagEight ==0) // 8.all 을 제외하고 최대 3개의 알고리즘을 선택할 수 있도록 조건 설정
			{
				printf("사용자 입력 에러 : 최대 3개의 값만 입력 가능합니다. 다시 시도하세요.\n");
				flag=1;
				break;
			}
		}
		
		if (flag==1) // 범위 안의 값을 입력하지 않았을 시 다시 메뉴 선택을 하도록 한다.
			continue;

		menu_b(page_frame, fin); // B 메뉴 호출
		input_data = menu_c(input_data, fin); // C 메뉴 호출
		do_menu_c(f1, f2, input_data, page_string, get_file_page_string); // input_data에 따른 데이터 입력 방식 처리 함수 

/*
			for (int i=0; i<10; i++) 
			{
				if (choiceMenu[i] != NULL) // 문자열 포인터 배열의 요소가 NULL값이 아니라면
					printf("%s\n", choiceMenu[i]);
			}
*/

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
	srand(time(NULL));
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
void menu_b(int page_frame, char fin[4])
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
void do_menu_c(FILE *f1, FILE *f2, int input_data, int page_string[500], char *get_file_page_string[500]) {
	// 데이터 입력 방식 1일 때.
	// 즉, 랜덤하게 생성할 때
	printf("do_menu_c의 input_data : %d\n", input_data);

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

		random_reference_string(page_string); // 랜덤으로 스트링 패턴 작성

		f1 = fopen("refer_str_file.txt", "w+");

		for(int j=0; j<500; j++)
			fprintf(f1, "%3d", page_string[j]);

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
			printf("%s ", get_file_page_string[m]);

		printf("\n");
		fclose(f2);
	}
}

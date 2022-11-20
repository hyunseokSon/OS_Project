#include "header.h"

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


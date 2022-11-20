#include "header.h"

int main()
{
    while (1)
    {
        char menu[50] = {0};        // 알고리즘 시뮬레이터 선택
        char *choiceMenu[10] = {0}; // 선택된 페이지 알고리즘
        int realMenu[10] = {0};     // 선택된 알고리즘 메뉴
        char fin[5] = "2625";       // 학번 20182625 중 뒤의 네자리 2625 입력시 프로그램 종료
        int i = 0, c;               // 조건문을 위한 변수
        int flag = 0;               // 조건을 충족하는 지 판별하기 위한 값
        int flagEight = 0;          // A. 메뉴 선택 시 8. All이 있는지 판별하기 위한 값
        int flagOpt = 0;            // 메뉴에서 Optimal 을 선택한 적이 있는지 판별하기 위한 값
        int page_string[500] = {0}; // 500개의 page string
        char refer_stringbit[500][7] = {0,}; // ESC - 참조비트가 적힌 참조 스트링 값이 저장됨.
        int page_frame = 0;         // page frame 개수
        int input_data;             // 데이터 입력 방식
        FILE *f1, *f2, *fp, *fp2;   // 파일 입력 및 출력을 위한 파일포인터
        Queue queue;                // LRU구현을 위한 QUEUE

        printf("A. Page Replacement 알고리즘 시뮬레이터를 선택하시오. (최대 3개 선택 가능, 종료 명령>어 : 2625)\n");
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
        int pageArray[page_frame];            // page frame값 만큼을 가지는 pageArray 배열 선언.

        input_data = menu_c(input_data, fin);       // C 메뉴 호출
        do_menu_c(f1, f2, input_data, page_string); //데이터 입력 방식 처리 함수
        ref_str_bit(page_string,refer_stringbit);   // ESC 구현을 위해 참조 스트림 뒤에 문자붙임

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


#include "header.h"

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


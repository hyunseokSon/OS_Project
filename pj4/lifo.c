#include "header.h"

// 3. LIFO
void lifo(int page_string[500], int page_frame, int *pageArray)
{
    FILE *fp;
    int t;                          //반복문을 위한 변수
    int index = -1;                 //페이지 배열 인덱스
    int fault = 0, flag_fault = 0;  // fault는 총 fault 수 count,
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


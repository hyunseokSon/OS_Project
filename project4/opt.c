#include "header.h"

// 1번 메뉴 : Optimal
void opt(int page_string[500], int page_frame, int *pageArray)
{
    FILE *fp;
    int index = -1;         // 페이지 배열 인덱스
    int fault = 0, flag_fault = 0;  //fault : fault일 때 개수, flag_fault : fault일 때의 flag값
    int t;                  // 반복문을 위한 변수
    int block = 0;          // 페이지 프레임 개수만큼 반복을 수행하기 위한 변수
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


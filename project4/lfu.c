#include "header.h"

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
		counter[t] = 0;
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
            if (fault < page_frame) // pageArray에 값이 다 채워지지 않았을 때
            {
                pageArray[index] = page_string[t];
                counter[index] = 1; // 값이 갱신되므로 counter는 1이 된다.
                index = (index+1) % page_frame;
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
            }
            fault++;
            flag_fault = 1;
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


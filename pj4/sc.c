#include "header.h"

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


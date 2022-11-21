#include "header.h"

// Enhanced Second chance
void esc(int page_string[500], int page_frame, int *pageArray, char r_s_b[500][7]) // esc
{
    FILE *fp;
    int t,c;                // 반복문을 위한 변수
    int flag_hit, fault=0;  // 
    int flag_write;         // W비트라면 1, 아니면 0
    int count;              // page_frame만큼 비교를 위한 변수.
    int check_ref;          // refernce bit값 확인을 위한 변수.
    int check_one;          // 1단계 검사(0,0)을 위한 변수.
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


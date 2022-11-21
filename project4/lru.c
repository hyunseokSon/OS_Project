#include "header.h"

// 4. LRU
void lru(int page_string[500], int page_frame, Queue *queue, int *pageArray)
{
    FILE *fp;
    int fault = 0, flag_fault = 0; //fault는 총 발생 횟수 count, flag_fault는 fault/hit 구분 flag
    int t,c; // 반복문을 위한 변수

    printf("-----------------------------------------");
    printf("\n (4). LRU \n");
    printf("-----------------------------------------");
    printf("\n참조스트림");

    fp = fopen("20182625.txt", "a");
    fprintf(fp,"-----------------------------------------");
    fprintf(fp,"\n (4). LRU \n");
    fprintf(fp,"-----------------------------------------");
    fprintf(fp,"\n참조스트림");

    initQueue(queue); // 큐 초기화
    Node *result = (Node *)malloc(sizeof(Node));

    // 페이지 배열 값을 -1로 초기화
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
        result = queue->front;

        // hit일 때
        if (searchQueue(queue, page_string[t]))
        {
            hitQueue(queue, page_string[t]);
            flag_fault = 0;
            result = queue->front;
            for (c = 0; c < page_frame; c++)
            {
                //data를 페이지 배열에 써주고 노드를 다음 곳으로 가리킴
                if (result != NULL)
                {
                    pageArray[c] = result->data;
					result = result->next;
                }
            }
        }

        // fault일 때
        else
        {
            //페이지 프레임 만큼 노드가 만들어지지 않았을 때
            if (queue->count < page_frame)
                pageArray[t] = getQueue(queue,page_string[t]);

            else
            {
                deQueue(queue);
                getQueue(queue, page_string[t]);
                result = queue->front; //dequeue에서 front가 바뀐다.
                for (c = 0; c < page_frame; c++)
                {
                    if (result != NULL)
                    {
                        pageArray[c] = result->data;
                        result = result->next;
                    }
                }
            }
            fault++;
            flag_fault = 1;
        }

        //결과 과정 출력
        printf("    %d\t", page_string[t]);
        fprintf(fp,"    %d\t", page_string[t]);

        // page_frame 만큼은 아래와 같이 출력
        for (c = 0; c < page_frame; c++)
        {
            printf("%11d\t", pageArray[c]);
            fprintf(fp,"%11d\t", pageArray[c]);
        }
        if (flag_fault) {
            printf("\tPAGE FAULT\n");
            fprintf(fp,"\tPAGE FAULT\n");
        }
        else {
			printf("\t          \n");
            fprintf(fp,"\t          \n");
        }

        flag_fault = 0;
    }

    printf("\nLRU: \n");
    printf("\tPage faults : %d\n", fault);
    printf("\t       Hits : %d\n", 500 - fault);
    printf("\t  Hit Ratio : %.2f%%\n", (500 - fault) * 100.0 / 500);
    printf("-----------------------------------------\n\n");

    fprintf(fp,"\nLRU: \n");
    fprintf(fp,"\tPage faults : %d\n", fault);
    fprintf(fp,"\t       Hits : %d\n", 500 - fault);
    fprintf(fp,"\t  Hit Ratio : %.2f%%\n", (500 - fault) * 100.0 / 500);
    fprintf(fp,"-----------------------------------------\n\n");
    fclose(fp);
}


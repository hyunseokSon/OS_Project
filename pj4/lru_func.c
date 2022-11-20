#include "header.h"

// 큐 초기화
void initQueue(Queue *queue)
{
    queue->front = queue->rear = NULL; // front와 rear값 NULL로 초기화
    queue->count = 0;                  // 보관 개수 0으로 초기화
}

// 큐가 비었는지 확인.
int isEmpty(Queue *queue)
{
    return queue->count == 0; //보관 개수가 0이면 빈 상태를 의미
}

//큐에서 새로운 node값 가져오기.
int getQueue(Queue *queue, int data)
{
    Node *now = (Node *)malloc(sizeof(Node)); // 노드를 생성
    now->data = data;                         // 데이터 지정
    now->next = NULL;

    if (isEmpty(queue)) //큐에 노드가 없을 때(비어있을 때)
    {
        queue->front = now; //맨 앞이 now가 됨.
    }
    else //노드가 있을 때(비어있지 않을 때)
    {
        queue->rear->next = now; //맨 마지막의 다음을 now로 설정
    }
    queue->rear = now; //맨 뒤를 now로 지정
    queue->count++;    //보관 개수 1 증가
    return now->data;
}

//큐에서 node 삭제
void deQueue(Queue *queue)
{
    Node *now;
    if (!isEmpty(queue))      // 큐가 비어있지 않을 때
    {
    now = queue->front;       //맨 앞의 노드를 now로 지정
    queue->front = now->next; //맨 앞은 now의 다음 노드로 지정
    free(now);                // now 메모리 해제
    queue->count--;           //보관 개수 1 감소
    }
}

// 큐에서 검색
int searchQueue(Queue *queue, int value)
{
    Node *search;
    search = queue->front;  //Queue의 맨 앞에서 시작

    while (search != NULL)
    {
        if (search->data == value)  //해당 값을 찾았을 때
            return 1;

        search = search->next;
    }
    return 0;               //해당 값을 찾지 못했을 때
}

// LRU에서 hit 발생 시 처리 함수
void hitQueue(Queue *queue, int value)
{
    Node *n;
    n = queue->front;

    // 처음 node에 일치하는 값이 있을 때 삭제
    if (n->data == value)
    {
        queue->front = n->next;
		free(n);
        queue->count--;
        //삭제 후 노드 추가
        getQueue(queue, value);
        return;
        //앞에 값 추가
    }

    // 중간 node에 일치하는 값이 있을 때 삭제
    else
    {
        while (n->next != NULL)
        {
            if (n->next->data == value && n->next->next != NULL)
            {
                Node *tmp = n->next;
                n->next = n->next->next;
                queue->count--;
                free(tmp); // 메모리 해제
                getQueue(queue, value);
                return;
            }
            n = n->next;
        }
    }
}


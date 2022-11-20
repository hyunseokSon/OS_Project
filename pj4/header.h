#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef struct node 
{
    int data; // 노드에 들어갈 데이터
    struct node *next; // 다음 노드를 가리킴
} Node;

typedef struct Queue
{
    Node *front; //맨 앞(꺼낼 위치)
    Node *rear;  //맨 뒤(보관할 위치)
    int count; // 노드 개수
} Queue;

void random_reference_string(int page_string[]); //무작위로 참조 스트림 만드는 함수
void cleanBuffer(void); // 입력 버퍼 비워주는 함수
int menu_b(int, char fin[4]); // B 메뉴 호출 함수
int menu_c(int, char fin[4]); // C 메뉴 호출 함수
void do_menu_c(FILE *f1, FILE *f2, int input_data, int page_string[500]); // C의 데이터 입력 방식에 >따른 처리 함수
int check_index(int page_frame, int *pageArray, int n); // 참조 스트림 값이 페이지에 있는지 확인하는 함수
int nearIndex(int pf, int cur, int *array, int n); //가장 가까이 있는 값 확인
int checkMax(int pf, int *pageArray); // 가장 멀리 있는 값 확인
void opt(int page_string[500], int page_frame, int *pageArray); // opt
void fifo(int page_string[500], int page_frame, int *pageArray); // fifo
void lifo(int page_string[500], int page_frame, int *pageArray); // lifo
void initQueue(Queue *queue); // LRU - Queue 초기화하는 함수
int isEmpty(Queue *queue); // LRU - Queue가 비어있는지 확인
int getQueue(Queue *queue, int data); // LRU - 큐에 노드 추가.
void deQueue(Queue *queue); // LRU - 큐에 노드 삭제
int searchQueue(Queue *queue, int value); // LRU - 큐에 해당 값을 가진 노드가 있는지 탐색
void hitQueue(Queue *queue, int value); // LRU - hit시 처리 함수
void lru(int page_string[500], int page_frame, Queue *queue, int *pageArray); // lru
void lfu(int page_string[500], int page_frame, int *pageArray); // lfu
void sc(int page_string[500], int page_frame, int *pageArray); // sc
void ref_str_bit(int page_string[500], char r_s_b[500][7]);
void esc(int page_string[500], int page_frame, int *pageArray, char r_s_b[500][7]); // esc


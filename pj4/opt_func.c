#include "header.h"

// 페이지 프레임에 참조 스트링 값이 있는지 확인하는 함수
int check_index(int page_frame, int *pageArray, int n)
{
    for (int k = 0; k < page_frame; k++)
    {
        if (pageArray[k] == n)
            return 1;
    }

    return 0;
}

// 가장 가까이 있는 것 확인
int nearIndex(int total, int cur, int *array, int n)
{
    // tatal은 전체 참조 스트림 갯수, cur은 현재 위치,
    // array는 참조 배열, n은 페이지에 있는 값을 의미한다.
    int a = cur;
    //해당 시점부터 500개의 참조 스트링까지 반복
    for (a++; a < total; a++)
    {
        if (array[a] == n)
        {
            return a;
        }
    }
    return a;
}

// 가장 멀리 있는 값을 확인하는 함수
int checkMax(int pf, int *pageArray)
{
    //pf는 page_frame을 의미.
    int ch = 0;
    int n = 0;
    int big = pageArray[0];

    //페이지 프레임 수 만큼 반복
    for (n++; n < pf; n++)
    {
        if (pageArray[n] > big)
        {
            big = pageArray[n];
            ch = n;
        }
    }
    return ch;
}


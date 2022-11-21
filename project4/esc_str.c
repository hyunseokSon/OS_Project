#include "header.h"

// 랜덤 스트림 값에 R 또는 W 비트를 추가하여 문자열 배열로 만들어 주는 함수
void ref_str_bit(int page_string[500], char r_s_b[500][7])
{
    int i;
    char d[2] = {0};

    for(i=0; i<500; i++)
    {
        // 참조 스트링의 숫자를 저장
        sprintf(r_s_b[i], "%d", page_string[i]);
        strcat(r_s_b[i], "(");
        int t= rand() % 2;
        if (t%2 ==0)
            d[0] = 'R';
        else
            d[0] = 'W';
        strcat(r_s_b[i], d);
        strcat(r_s_b[i], ")");
    }
}


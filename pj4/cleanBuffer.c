#include "header.h"

// 입력 버퍼 비워주는 함수.
void cleanBuffer(void)
{
    while (getchar() != '\n')
        ;
}


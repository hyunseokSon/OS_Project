#include "header.h"

// 1~30을 500번 돌린 후 page_string배열에 넣는다.
void random_reference_string(int page_string[500])
{
    srand(time(NULL));
    int random = 0;
    for (int n = 0; n < 500; n++)
    {
        random = rand() % 30 + 1; // 1~30 random
        page_string[n] = random;
    }
}


#include <stdio.h>

 struct stri{
	int t[3];
};

int main()
{
	struct stri st[3];
	st[0].t[0] = 6;
	st[0].t[1] = 8;
	st[0].t[2] = 10;

	for (int i=0; i<3; i++)
		printf("%d: %d\n", i+1,st[0].t[i]);

	return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void do_file(FILE *f1, FILE *f2, int a[10]);

int main()
{
	int a[10]={0};
	FILE *f1, *f2;

	srand(time(NULL)); 

	printf("1~30 10번 출력하기\n");
	int random=0;
	for (int i=0; i<10; i++) {
	random = rand() % 30 + 1; //1~30
	a[i] = random;
	}

	printf("\n\n");

	for (int i=0; i<10; i++)
		printf("%3d", a[i]);
	do_file(f1,f2,a);

	return 0;
}

void do_file(FILE *f1, FILE *f2, int a[10])
{

	f1 = fopen("reference_string_file.txt", "w+");

	for (int i=0; i<10; i++)
		fprintf(f1, "%3d", a[i]);

	if (f1 ==NULL)
	{
		printf("파일 열기 실패");
	} 
	else {
		printf("파일 열기 성공");
	}
	fclose(f1);

	f2 = fopen("reference_string_file.txt", "r+");
	char a2[2000]={0};
	fread(a2,1,2000,f2);
	printf("%s\n", a2);
	fclose(f2);
}

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char userID[16][32];
char pwdID[16][32];

void get_user_list() 
{
	int fd;
	int userNum = 0;
	int flag = 0;

	fd = open("list.txt", O_RDONLY);

	if (fd < 0) 
	{
		printf(1, "file cannot open..\n");
		flag = 1;
		exit();
	}



	for(i=0; i< 16; i++)
	{
		// store the userID, pwdID about Username, Password info in list.txt
		if (flag)
			break;
		userNum++;
		for (int j=0; ; j++) {
			if (read(fd, userID[j], 1) <= 0) 
			{
				flag=1;
				break;
			}

			if ( userID[j] == ' ' || userID[j] == '\n') 
			{
				userID[j] = '\0';
				break;
			}
				
		}
	}


}

int check_idpw() 
{
//compare Username, Password with list.txt
	char checkID[32] = {0};
	char checkPW[32] = {0};

	printf(1, "Username : ");
	
}

int main(int argc, char *argv[])
{
	return 0;
}

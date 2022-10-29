#include "types.h"
#include "user.h"

int main(int argc, char **argv) 
{
	int res = atoi(argv[1]);

	trace(res);
	exec(argv[2], &argv[2]);
	wait();
}

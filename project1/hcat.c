#include "types.h"
#include "stat.h"
#include "user.h"

int line=0;
char buf[1];

void
hcat(int fd)
{
  int n;
  int count=0;

  while((n = read(fd, buf, sizeof(buf))) > 0) 
  { //they can be read
    if (write(1, buf, n) != n) {
      printf(1, "cat: write error\n");
      exit();
    }

	if(buf[0] == '\n'){
	  count++;
	  if(count == line) 
	  {
		  break;
  }
}
}
	if(n<0) {
	printf(1, "cat: read error\n");
    exit();
  }
}

int
main(int argc, char *argv[])
{
  int fd, i;

  if(argc <= 2){
    hcat(0);
    exit();
  }

  line = atoi(argv[1]);

  for(i = 2; i < argc; i++){
    if((fd = open(argv[i], 0)) < 0){
      printf(1, "cat: cannot open %s\n", argv[i]);
      exit();
    }
   hcat(fd);
    close(fd);
  }
  exit();
}

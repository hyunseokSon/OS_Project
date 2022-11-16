CC = gcc
TARGET = start

$(TARGET) : job.o hobby.o main.o
			$(CC) -o $(TARGET) job.o hobby.o main.o

job.o : job.c
	$(CC) -c -o job.o job.c
  
hobby.o : hobby.c
	$(CC) -c -o hobby.o hobby.c
  
main.o : main.c
	$(CC) -c -o main.o main.c
  
clean : 
	rm *.o start

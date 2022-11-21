CC = gcc
TARGET = project4
OBJECTS = lfu.o lru.o main.o opt.o sc.o esc.o fifo.o lifo.o lru_func.o menu.o opt_func.o random_str.o esc_str.o

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) -o $@ $^
  
clean : 
	rm *.o project4

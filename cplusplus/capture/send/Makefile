CC = gcc
XX = g++
CFLAGS = -Wall -O -g -I../include/libnet -I../aes
LFLAGS = -L../lib -lnet
TARGET = send

SOURCES = $(wildcard *.c *.cpp ../aes/*.c ../aes/*.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))

$(TARGET) : $(OBJS)
	$(XX) -o $(TARGET) $(OBJS) $(CFLAGS) $(LFLAGS) 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o:%.cpp
	$(XX) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o ${TARGET}

CC = gcc
XX = g++
CFLAGS = -Wall -O -g -I../include/pcap -I../aes
LFLAGS = -L../lib -lpcap -static -ldbus-glib-1 -ldbus-1 -lpthread -lrt
TARGET = pcap

SOURCES = $(wildcard *.c *.cpp ../aes/*.c ../aes/*.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))

$(TARGET) : $(OBJS)
	$(XX) -o $(TARGET) $(OBJS) $(CFLAGS) $(LFLAGS) 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o:%.cpp
	$(XX) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(TARGET)

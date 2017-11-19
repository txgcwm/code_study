TARGET = select poll epoll client

CC = gcc
CXX = g++
CFLAGS =
LDFLAGS =

SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %cpp, %o, $(SRCS))
  


all: $(TARGET)
  
select: select.o common.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

poll: poll.o common.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

epoll: epoll.o common.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

client: client.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c $^ -o $@

%.o : %.cpp
	$(CXX) $(CFLAGS) -c $^ -o $@

.PHONY: clean

clean :  
	rm -f *.o ${TARGET}
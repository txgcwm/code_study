TARGET = deque list vetor map multimap set multiset 

CC = gcc
CXX = g++
CFLAGS =
LDFLAGS =

SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %cpp, %o, $(SRCS))
  


all: $(TARGET)
  
deque: deque.o 
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

list: list.o 
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

vetor: vetor.o 
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

map: map.o 
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

multimap: multimap.o 
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

set: set.o 
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

multiset: multiset.o 
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c $^ -o $@

%.o : %.cpp
	$(CXX) $(CFLAGS) -c $^ -o $@

.PHONY: clean

clean :  
	rm -f *.o ${TARGET}
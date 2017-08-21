OBJS_READ = pipe_read.o
OBJS_WRITE = pipe_write.o


CFLAGS = -c
CC = g++

PROS = pipe_read pipe_write

all: $(PROS)

.o:.cpp
	$(CC) $(CFLAGS) $<

pipe_write: $(OBJS_WRITE)
	$(CC) -o $@ $^

pipe_read: $(OBJS_READ)
	$(CC) -o $@ $^

clean:
	rm -rf $(PROS) *.o

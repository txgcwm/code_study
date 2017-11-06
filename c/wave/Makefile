LIBWAVE = libwave.a
EXECUTABLE = wave

CFLAGS = -Wall -g 
CFLAGS += -I ./
LDFLAGS += -L ./
LDLIBS += -lwave

SRCS = wave_header.c wave_parse.c
OBJS = $(SRCS:.c=.o)

TEST_SRCS = main.c
TEST_OBJS = $(TEST_SRCS:.c=.o)

all:$(LIBWAVE) $(EXECUTABLE)

$(LIBWAVE):$(OBJS)
	$(AR) -rc $(LIBWAVE) $(OBJS)

$(EXECUTABLE):$(TEST_OBJS)
	$(CC) -o $(EXECUTABLE) $(CFLAGS) $(TEST_OBJS) $(LDFLAGS) $(LDLIBS) -static

%.o:%.c
	$(CC) -c -o $@ $< $(CFLAGS) 

clean:
	rm -f $(EXECUTABLE) *.[oa]

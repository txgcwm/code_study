CROSSPREFIX=
CC=${CROSSPREFIX}gcc
CXX=${CROSSPREFIX}g++
LD=${CROSSPREFIX}ld
AR=${CROSSPREFIX}ar
NM=${CROSSPREFIX}nm
RANLIB=${CROSSPREFIX}ranlib
STRIP=${CROSSPREFIX}strip

ELF = airkiss
SRCS = main.c 
SRCS += capture/common.c capture/osdep.c capture/linux.c capture/radiotap/radiotap-parser.c
SRCS += utils/wifi_scan.c airkiss.c
OBJS = $(patsubst %.c,%.o,$(SRCS))

CFLAG += -I 3rdparty/prebuild/wireless_tools/include/
LDFLAG += -L3rdparty/prebuild/wireless_tools/lib/
LIBIW = -liw
TIMER = -lrt -lm
CCFLAGS = -c -g -Wall -Wno-unused-but-set-variable

all: $(ELF)

$(ELF) : $(OBJS)
	$(CC) $^ -o $@ $(LDFLAG) $(LIBIW) $(TIMER)

$(OBJS):%.o:%.c
	$(CC) $(CFLAG) $(CCFLAGS) $< -o $@

clean:
	rm -f  $(ELF) $(OBJS)

.PHONY: all clean

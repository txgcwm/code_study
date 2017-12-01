LIBMQTTLINK=libmqttlink.a
LIBMQTTLINK_SHARED=libmqttlink.so
LIBMQTTLINK_VERS=0.1

LOCAL_PATH=${shell pwd}

CFLAGS = -fPIC -I ./ -I ../deps/paho.mqtt.c/src/ -I ../include
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)


all: $(OBJS)
	$(AR) -rc $(LIBMQTTLINK) $(OBJS)
	$(CXX) -shared -fPIC -o $(LIBMQTTLINK_SHARED) $(OBJS)

%.o: %.cpp
	${CXX} ${CFLAGS} -c $^ -o $@

clean:
	rm -f *.[oa] *.so
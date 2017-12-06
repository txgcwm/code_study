BIN=mqtt_test

CFLAGS  += -I../include
LDLIBS += -L./lib -lmqttlink -lpaho-mqtt3cs
SRCS=mqtt_test.cpp
OBJS = $(SRCS:.cpp=.o)

$(BIN): $(OBJS)
	$(CXX) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

%.o: %.cpp
	${CXX} ${CFLAGS} -c $^ -o $@

clean:
	rm -f $(BIN) *.o
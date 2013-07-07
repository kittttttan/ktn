CXX = g++ #clang++
CFLAGS = -g -W -Wall -O2 -std=c++11
TARGET = test
OBJS = main.o test.o ulong.o long.o fraction.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

clean:
	-$(RM) $(TARGET) $(OBJS)

.cc.o:
	$(CXX) $(CFLAGS) -c $<

main.o:
ulong.o:
long.o:
fraction.o:
test.o:

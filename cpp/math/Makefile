CXX = g++ #clang++
CFLAGS = -g -Wall -O2
TARGET = test
OBJS = test.o ulong.o long.o fraction.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

clean:
	-$(RM) $(TARGET) $(OBJS)

.cc.o:
	$(CXX) $(CFLAGS) -c $<

ulong.o: ulong.h
long.o: long.h
fraction.o: ulong.h
test.o: long.h fraction.h complex.h

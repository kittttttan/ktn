CXX = g++ #clang++
CFLAGS = -g -Wall -O2 -Dnullptr=NULL
TARGET = test
OBJS = main.o test.o ulong.o long.o fraction.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

clean:
	-$(RM) $(TARGET) $(OBJS)

.cc.o:
	$(CXX) $(CFLAGS) -c $<

main.o: test.h
ulong.o: ulong.h
long.o: long.h
fraction.o: ulong.h
test.o: long.h fraction.h complex.h vector.h matrix.h

CXX = g++
CFLAGS = -g -Wall -O2
TARGET = test.exe
OBJS = test.o ulong.o long.o fraction.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

clean:
	-$(RM) $(TARGET) $(OBJS)

.cpp.o:
	$(CXX) $(CFLAGS) -c $<

ulong.o: ulong.h
long.o: long.h
fraction.o: ulong.h
test.o: long.h fraction.h

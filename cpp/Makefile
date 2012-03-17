CXX = g++
CFLAGS = -g -Wall -O2 -Dnullptr=NULL -D_UNICODE
TARGET = test
OBJS = test.o string.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

clean:
	-$(RM) $(TARGET) $(OBJS)

.cc.o:
	$(CXX) $(CFLAGS) -c $<

string.o: string.h tchar.h
test.o: string.h

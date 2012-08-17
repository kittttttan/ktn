CXX = g++
CFLAGS = -g -Wall -O2 -Dnullptr=NULL
TARGET = test
OBJS = test.o string.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

clean:
	-$(RM) $(TARGET) $(OBJS)

.cc.o:
	$(CXX) $(CFLAGS) -c $<

string.o: string.h util.h dbg.h
test.o: string.h array.h dbg.h logging.h

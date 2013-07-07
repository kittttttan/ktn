CXX = g++
CFLAGS = -g -W -Wall -O2 -Dnullptr=NULL
TARGET = test
OBJS = test.o string.o stringw.o date_format.o date.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

clean:
	-$(RM) $(TARGET) $(OBJS)

.cc.o:
	$(CXX) $(CFLAGS) -c $<

string.o:
stringw.o:
date_format.o:
date.o:
test.o:

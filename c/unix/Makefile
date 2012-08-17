CC = gcc
CFLAGS = -g -Wall -O2
TARGET = test
OBJS = test.o string.o logger.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

clean:
	-$(RM) $(TARGET) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $<

logger.o:
string.o:
test.o:

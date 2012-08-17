CC = gcc
CFLAGS = -g -Wall -O2
TARGET = test
OBJS = main.o long.o test.o ../string.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

clean:
	-$(RM) $(TARGET) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $<

main.o:
long.o: ../string.o
test.o:

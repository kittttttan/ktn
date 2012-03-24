CC = clang #gcc
CFLAGS = -g -Wall -O2
TARGET = test
OBJS = main.o ulong.o test.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

clean:
	-$(RM) $(TARGET) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $<

main.o: test.h
ulong.o: ulong.h ../bool.h
test.o: test.h

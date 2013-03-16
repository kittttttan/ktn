CC = gcc
CFLAGS = -g -Wall -Wextra -O2
TARGET = test
OBJDIR = obj/
SOURCES = $(wildcard *.c)
OBJS = $(addprefix $(OBJDIR), $(SOURCES:%.c=%.o))

all: $(TARGET)

clean:
	-$(RM) $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

$(OBJDIR)%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

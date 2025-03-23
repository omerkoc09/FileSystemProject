CC=gcc
CFLAGS=-Wall -Wextra -g
TARGET=file_system
SRCS=main.c file_operations.c directory_operations.c logger.c utils.c
OBJS=$(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS) log.txt 
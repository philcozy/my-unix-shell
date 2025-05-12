CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = main.c parser.c builtin.c exec.c signals.c pipe.c
OBJS = $(SRCS:.c=.o)

TARGET = myshell

all: $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lreadline

%.o: %.c shell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

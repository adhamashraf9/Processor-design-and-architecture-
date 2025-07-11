CC = gcc
CFLAGS = -Wall -Wextra
TARGET = simulator.exe
SRCS = main.c instruction.c memory.c pipeline.c execute.c utils.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del *.o $(TARGET)

.PHONY: all clean 
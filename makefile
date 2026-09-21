CC = gcc
CFLAGS =

SRCS := state_machine.c main.c
INC := 

OBJS := $(SRCS:.c=.o)

TARGET_NAME = main.exe

all: $(OBJS)
	$(CC) $(CFLAGS) $(^) -o $(TARGET_NAME)  

%.o : %.c
	$(CC) $(CFLAGS) -c $(^) -o $(@)

clean:
	del *.o *.exe
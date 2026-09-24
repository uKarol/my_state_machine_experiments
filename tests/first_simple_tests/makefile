CC = gcc
CFLAGS =

STATE_MACHINE_PATH = ../../state_machine/

SRCS := $(STATE_MACHINE_PATH)state_machine.c main.c
INC := -I$(STATE_MACHINE_PATH)

OBJS := $(SRCS:.c=.o)

TARGET_NAME = main.exe

all: $(OBJS)
	$(CC) $(CFLAGS) $(^) -o $(TARGET_NAME)  

%.o : %.c
	$(CC) $(CFLAGS) -c $(^) $(INC) -o $(@)

clean:
	del *.o *.exe
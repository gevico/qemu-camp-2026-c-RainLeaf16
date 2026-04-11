CC = gcc
CFLAGS = -Wall -g -O2 -fPIE

TARGET := 17_myfile
SRCS := 17_myfile.c
OBJS := $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): 17_myfile.o
	$(CC) $(CFLAGS) -pie -o $(TARGET) 17_myfile.o

17_myfile.o: 17_myfile.c
	$(CC) $(CFLAGS) -c 17_myfile.c -o 17_myfile.o

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean

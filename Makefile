CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = scheduler
SRC = scheduler.c
OBJ_DIR = obj

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

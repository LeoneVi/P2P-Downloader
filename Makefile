CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = P2PDownloader
SRC = P2PDownloader.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
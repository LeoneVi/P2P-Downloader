CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

TARGET = P2PDownloader
SRC = P2PDownloader.c torrent.c
OBJ = P2PDownloader.o torrent.o

ZIP = P2PDownloader.zip

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

zip:
	zip $(ZIP) $(SRC) torrent.h Makefile

clean:
	rm -f $(TARGET) $(OBJ) $(ZIP)
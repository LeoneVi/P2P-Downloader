CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = P2PDownloader
SRC = P2PDownloader.c
ZIP = P2PDownloader.zip

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

zip:
	zip $(ZIP) $(SRC) Makefile

clean:
	rm -f $(TARGET) $(ZIP)
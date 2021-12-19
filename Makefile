CC = g++
CFLAGS = -g -Wall -std=c++11 -stdlib=libc++ -lc++ -O3
TARGET = ./bin/program

all:
	mkdir -p ./bin
	$(CC) $(CFLAGS) -o $(TARGET) ./src/main.cpp

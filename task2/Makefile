CC = g++
CFLAGS = -std=c++11 -O2

main: main.cpp $(shell find searchers -type f)
	$(CC) $(CFLAGS) main.cpp -o main

clean:
	rm main

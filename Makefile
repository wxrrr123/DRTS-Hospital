.PHONY: build run

all: build run

build:
	g++ -std=c++17 -Iinclude -o DRTS.o main.cpp patient.cpp vehicle.cpp system.cpp subsystem.cpp GA.cpp

run:
	./DRTS.o > result.txt

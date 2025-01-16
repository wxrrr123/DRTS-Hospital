.PHONY: build run

all: build run

build:
	g++ -std=c++17 -Iinclude -o DRTS main.cpp patient.cpp vehicle.cpp system.cpp subsystem.cpp GA.cpp

run:
	./DRTS > result.txt

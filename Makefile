.PHONY: build run

all: build run

build:
	g++ -std=c++17 -o DRTS main.cpp patient.cpp vehicle.cpp system.cpp

run:
	./DRTS

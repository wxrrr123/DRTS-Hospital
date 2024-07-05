.PHONY: build run

all: build run

build:
	g++ -o DRTS main.cpp patient.cpp vehicle.cpp system.cpp

run:
	./DRTS

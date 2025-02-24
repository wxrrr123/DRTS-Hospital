SRC_DIR := src
INC_DIR := include
BIN_DIR := bin
OUT_DIR := output
DATA_DIR := data

EXECUTABLE := $(BIN_DIR)/DRTS

TIMESTAMP := $(shell date +"%Y-%m-%d-%H%M%S")

OUTPUT_FILE := $(OUT_DIR)/simulation_$(TIMESTAMP).txt

.PHONY: build run clean

all: build run

build:
	mkdir -p $(BIN_DIR)
	g++ -std=c++17 -I$(INC_DIR) -pthread -g -O3 -o $(EXECUTABLE) $(SRC_DIR)/main.cpp $(SRC_DIR)/patient.cpp $(SRC_DIR)/vehicle.cpp $(SRC_DIR)/system.cpp $(SRC_DIR)/subsystem.cpp $(SRC_DIR)/GA.cpp

run:
	mkdir -p $(OUT_DIR)
	$(EXECUTABLE) $(DATA_DIR) > $(OUTPUT_FILE)

clean:
	rm -rf $(BIN_DIR)/* $(OUT_DIR)/*

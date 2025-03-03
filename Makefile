SRC_DIR := src
BIN_DIR := bin
OUT_DIR := output

CXXFLAGS := -std=c++17 -Iinclude -pthread -O3
TIMESTAMP := $(shell date +"%Y%m%d_%H%M%S")

EXE_FILE := $(BIN_DIR)/DRTS
SRC_FILE := $(wildcard $(SRC_DIR)/*.cpp)
OUT_FILE := $(OUT_DIR)/$(TIMESTAMP).txt

.PHONY: all build run clean

all: build run

build: $(EXE_FILE)

$(EXE_FILE): $(SRC_FILE)
	mkdir -p $(BIN_DIR)
	g++ $(CXXFLAGS) -o $@ $^

run:
	mkdir -p $(OUT_DIR)
	$(EXE_FILE) > $(OUT_FILE)

clean:
	rm -rf $(BIN_DIR) $(OUT_DIR)

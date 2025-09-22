.PHONY : all clean

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

TARGET := $(BIN_DIR)/reverse

$(TARGET): $(OBJS) | $(BIN_DIR)
	gcc -o $@ $(OBJS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	gcc -c $< -o $@

$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

test:
	./tests/run.sh

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

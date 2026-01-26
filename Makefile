CC := gcc
CFLAGS := -Iinclude -O2 -mavx2 -pthread

DEPFLAGS := -MMD -MP
LDFLAGS := -lm -pthread

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

TARGET_NAME := radiation
TARGET := $(BIN_DIR)/$(TARGET_NAME)

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
DEPS := $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	@echo "Linked $@."
	@$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo "Compiled $<."
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(BIN_DIR) $(BUILD_DIR):
	@mkdir -p $@ output output-image

clean-output:
	@rm -rf output output-image

-include $(DEPS)

clean:
	@echo "Removed object and binary files."
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean clean-output
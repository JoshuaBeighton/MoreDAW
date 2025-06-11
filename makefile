# Compiler and flags
CC = gcc
CFLAGS = -Wall -I./src/backend -I./src/frontend `pkg-config --cflags gtk4`
LDFLAGS = ./src/backend/libportaudio.a `pkg-config --libs gtk4` -lm -lasound -ljack


# Directories
BACKEND_DIR = src/backend
FRONTEND_DIR = src/frontend
BUILD_DIR = build

# Source and object files
BACKEND_SRCS = $(wildcard $(BACKEND_DIR)/*.c)
FRONTEND_SRCS = $(wildcard $(FRONTEND_DIR)/*.c)
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(BACKEND_SRCS) $(FRONTEND_SRCS)))

# Output executable
TARGET = moreDAW

# Default target
all: $(BUILD_DIR) $(TARGET)

# Build target
$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

# Compile source files into object files
$(BUILD_DIR)/%.o: $(BACKEND_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(FRONTEND_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Run the application
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

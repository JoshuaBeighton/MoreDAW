# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -I./src/backend -I./src/frontend `pkg-config --cflags gtk4`
LDFLAGS = -g ./src/backend/libportaudio.a `pkg-config --libs gtk4` -lm -lasound -ljack -pthread

# Directories
BACKEND_DIR = src/backend
FRONTEND_DIR = src/frontend
BUILD_DIR = build

# Source and object files
BACKEND_SRCS = $(wildcard $(BACKEND_DIR)/*.c)
FRONTEND_SRCS = $(wildcard $(FRONTEND_DIR)/*.c)
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(BACKEND_SRCS) $(FRONTEND_SRCS))) \
       $(BUILD_DIR)/resources.o

# Output executable
TARGET = moreDAW

# Default target
all: $(BUILD_DIR) $(TARGET)

# Build target
$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

# Compile resource file
$(BUILD_DIR)/resources.c: resources.xml
	glib-compile-resources $< --target=$@ --generate-source

$(BUILD_DIR)/resources.o: $(BUILD_DIR)/resources.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile backend source files
$(BUILD_DIR)/%.o: $(BACKEND_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile frontend source files
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

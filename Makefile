CC       := g++
CFLAGS   := -DARCH_X86_64 -Wall -Wextra -g -pg -O2
LDFLAGS  := -L/usr/lib -fopenmp -lm
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
TARGET   := run
INCLUDE  := -Iinclude/
SRC      := $(wildcard src/*.cpp)

OBJECTS := $(SRC:src/%.cpp=$(OBJ_DIR)/%.o)

all: build $(APP_DIR)/$(TARGET)

# Rule to create .o files in the build/objects directory
$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $< 

# Rule to create the executable in the build/apps directory
$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $(OBJECTS) $(LDFLAGS)

.PHONY: all build clean debug release

# Rule to build the necessary directories
build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

# Debug and release targets
debug: CFLAGS += -DDEBUG -O0
debug: all

release: CFLAGS += -O2
release: all

# Clean target to remove generated files
clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*

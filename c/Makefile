CXX = gcc
CXXFLAGS_COMMON = -std=c17 -Wall -Wextra -lm
CXXFLAGS_RELEASE = $(CXXFLAGS_COMMON) -Ofast
CXXFLAGS_DEBUG = $(CXXFLAGS_COMMON) -O0 -fsanitize=address -DDEBUG -g \
				 -Wno-sign-conversion -D_GLIBCXX_ASSERTIONS

MODE ?= debug
ifeq ($(MODE), debug)
$(warning MODE is debug)
	CXXFLAGS = $(CXXFLAGS_DEBUG)
else 
$(warning MODE is release)
	CXXFLAGS = $(CXXFLAGS_RELEASE)
endif

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .

SOURCES = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(SRC_DIR)/*.h)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
EXECUTABLE = $(BIN_DIR)/ray-tracer

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o

run: $(EXECUTABLE)
	./$(EXECUTABLE) <input.txt >draw.ppm

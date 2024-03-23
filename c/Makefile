CXX = gcc
CXXFLAGS_COMMON = -std=c17 -Wall -Wextra
CXXFLAGS_RELEASE = $(CXXFLAGS_COMMON) -Ofast
CXXFLAGS_DEBUG = $(CXXFLAGS_COMMON) -O0 -DDEBUG -g \
				 -Wno-sign-conversion -D_GLIBCXX_ASSERTIONS
				# -fsanitize=address
CXXFLAGS_LINK = -lm 

# make MODE=release
MODE ?= debug 
ifeq ($(MODE), debug)
# $(warning MODE is debug)
	CXXFLAGS = $(CXXFLAGS_DEBUG)
else 
# $(warning MODE is release)
	CXXFLAGS = $(CXXFLAGS_RELEASE)
endif

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(SRC_DIR)/*.h)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
EXECUTABLE = $(BIN_DIR)/ray-tracer

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS_LINK) $(OBJECTS) -o $@

# make clean 
.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o

# make MODE=release run 
run: $(EXECUTABLE)
	$(EXECUTABLE) <input.txt

EXECUTABLE_DENOISE = $(BIN_DIR)/denoise-pfm
DENOISER_DIR = denoiser
CXXFLAGS_LINK_DENOISER = -lOpenImageDenoise

$(EXECUTABLE_DENOISE):
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_LINK_DENOISER) $(DENOISER_DIR)/denoise-pfm.c -o $@

# make MODE=release den
den: $(EXECUTABLE_DENOISE)
	$(EXECUTABLE_DENOISE) >draw-denoised.ppm

CXX = gcc
CXXFLAGS_COMMON = -std=c17 -Wall -Wextra -lm
LDFLAGS_COMMON =

SRCS = main.c scanner.c algebra.c ray.c object.c sphere.c plane.c triangle.c \
	   camera.c draw.c
OBJS = $(SRCS:.c=.o)
TARGET = ray-tracer

CXXFLAGS_RELEASE = $(CXXFLAGS_COMMON) -Ofast
LDFLAGS_RELEASE = $(LDFLAGS_COMMON)

CXXFLAGS_DEBUG = $(CXXFLAGS_COMMON) -O0 -fsanitize=address -DDEBUG -g \
				 -Wno-sign-conversion -D_GLIBCXX_ASSERTIONS
LDFLAGS_DEBUG = $(LDFLAGS_COMMON)

MODE ?= debug

ifeq ($(MODE), debug)
$(warning MODE is debug)
	CXXFLAGS = $(CXXFLAGS_DEBUG)
	LDFLAGS = $(LDFLAGS_DEBUG)
else 
$(warning MODE is release)
	CXXFLAGS = $(CXXFLAGS_RELEASE)
	LDFLAGS = $(LDFLAGS_RELEASE)
endif

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) < input.txt

clean: 
	rm -f $(OBJS) $(TARGET)

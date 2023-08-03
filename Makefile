all: main

CXX = g++
override CXXFLAGS += -g -Wall -Wextra -pedantic

SRCS = $(shell dir /A-D /B /S *.cpp)
HEADERS = $(shell dir /A-D /B /S *.h)

main: $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o "$@"

main-debug: $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -O0 $(SRCS) -o "$@"

clean:
	del main.exe main-debug.exe

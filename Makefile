all: main

CXX = g++
override CXXFLAGS += -g -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wformat=2 -Winit-self \
						-Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast \
						-Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo \
						-Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef \
						-Wno-unused # -Wdisabled-optimization -Werror

SRCS = $(shell dir /A-D /B /S *.cpp)
HEADERS = $(shell dir /A-D /B /S *.h)

main: $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o "$@"

main-debug: $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -O0 $(SRCS) -o "$@"

format:
	clang-format -i $(SRCS) $(HEADERS)

clean:
	del main.exe main-debug.exe

INCL := -Iinclude
SRC_FILES := $(wildcard src/*.cpp)
CFLAGS := -std=c++11 -Wall
EXECUTABLE = final
EXEC := -o $(EXECUTABLE)

.PHONY: all clean

all: final

clean:
	-rm -f final

final: $(SRC_FILES)
	g++ $(EXEC) $(SRC_FILES) $(INCL) $(CFLAGS)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRC = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)
OBJ = $(SRC:src/%.cpp=bin/%.o)
TARGET = bin/ChronoRent.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

bin/%.o: src/%.cpp
	@if not exist "bin" mkdir bin
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ADD THIS LINE:
run: all
	@.\bin\ChronoRent.exe

clean:
	@if exist bin rd /s /q bin

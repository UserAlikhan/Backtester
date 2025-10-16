# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

# Files
SRCS = main.cpp csv_loader.cpp candle.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = backtester

# Default rule
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile .cpp into .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
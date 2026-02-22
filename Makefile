# Define the compiler
CXX = g++

# Define compiler flags (warnings and C++ standard)
CXXFLAGS = -Wall -Wextra -std=c++17

# Define the name of the final executable
TARGET = app

# Define the source files
SRCS = main.cpp qrcode.cpp

# Define the object files (automatically changes .cpp to .o)
OBJS = $(SRCS:.cpp=.o)

# Default rule when you just run 'make'
all: $(TARGET)

# Rule to link object files and create the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .cpp files into .o object files
# Including qrcode.h ensures recompilation if the header file changes
%.o: %.cpp qrcode.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to clean up the compiled files
clean:
	rm -f $(OBJS) $(TARGET)

# Tell Make that 'all' and 'clean' are not actual files
.PHONY: all clean
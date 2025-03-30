# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra
# Source files
SRCS = main.cpp Producer.cpp BoundedBuffer.cpp Dispatcher.cpp Unbounded_Buffer.cpp
# Object files
OBJS = $(SRCS:.cpp=.o)
# Executable name
TARGET = ex3.out

# Main target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up intermediate object files and executable
clean:
	rm -f $(OBJS) $(TARGET)



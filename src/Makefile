SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
BIN=automataReader
LIB_INC_PATH=/opt/repository/matrix/include
CXXFLAGS += -Wall -O0 -I$(LIB_INC_PATH) -g

all: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN) $(OBJ)

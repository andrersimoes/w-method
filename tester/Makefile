SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
BIN=wMethodTester
LIB=wmethod
LIB_PATH=-L../src/
INCLUDE= -I/opt/repository/matrix/include -I ../src/
CXXFLAGS += -Wall -O0 -g $(INCLUDE) $(LIB_PATH)

all: lib $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(LIB_PATH) -l $(LIB) -o $(BIN)
	./$(BIN)

lib: ../src/lib$(LIB).a

../src/lib$(LIB).a:
	$(MAKE) -C ../src lib

clean:
	rm -f $(BIN) $(OBJ)

distclean: clean
	$(MAKE) -C ../src distclean

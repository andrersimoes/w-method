SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
BIN=w-method
LIB=libwmethod
LIB_INC_PATH=/opt/repository/matrix/include
CXXFLAGS += -Wall -O0 -I$(LIB_INC_PATH) -g

all: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(BIN)
	./$(BIN)

lib: $(LIB).a

$(LIB).a: $(OBJ)
	ar crf $(LIB).a $(OBJ)

clean:
	rm -f $(BIN) $(OBJ)

distclean: clean
	rm -f *.a

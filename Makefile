CXXFLAGS = -std=c++17 -Wall -pedantic -fsanitize=address -MMD -g
LDFLAGS = -lGL -lGLU -lglut -lSDL2 -fsanitize=address -lstdc++fs

SRC = $(wildcard ./src/*.cpp)
OBJ = $(SRC:.cpp=.o)
HD = $(wildcard ./src/*.h)
DEPS = $(SRC:%.cpp=%.d)

.PHONY: clean compile run

all: midi-converter doc
compile: midi-converter
clean:
	rm -r doc/*
	rm $(OBJ) $(DEPS) midi-converter
	find examples/folder -name "*.json" -type f -delete
	find examples/folder -name "*_res.mid" -type f -delete
run: compile
	./midi-converter
doc: doc/index.html
doc/index.html: Doxyfile $(HD)
	doxygen Doxyfile
midi-converter: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

-include $(DEPS)

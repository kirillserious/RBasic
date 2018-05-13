CC=g++
CFLAGS=-c -std=c++11 -Wall
LDFLAGS=
SOURCES=main.cpp Exeption.cpp Scanner/Lex.cpp Scanner/Scanner.cpp Type/SimpleType.cpp Type/Functional.cpp Type/Part.cpp Type/Type.cpp Parser/Parser.cpp Interpreter/Interpreter.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=RBasic

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -r $(OBJECTS)
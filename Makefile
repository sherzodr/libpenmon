TARGET = example.exe

all: $(TARGET)

libpenmon.o: libpenmon.h libpenmon.cpp
	g++ -c libpenmon.cpp

$(TARGET): libpenmon.o example.cpp
	g++ -Wall -o $(TARGET) libpenmon.o example.cpp
	
test: all
	example.exe

clean:
	rm -f *.exe *.o *~ $(TARGET)

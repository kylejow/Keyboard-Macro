all: main
CXXFLAGS=-Wall -g
CXX=g++
main: main.o display.o program.o
	$(CXX) -o $@ $^
clean:
	rm -f *.o *.exe
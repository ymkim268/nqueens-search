search: input.o node.o main.o
	g++ input.o node.o main.o -o search

main.o: main.cpp input.h node.h
	g++ -c main.cpp

input.o: input.cpp input.h
	g++ -c input.cpp

node.o: node.cpp node.h
	g++ -c node.cpp

clean:
	rm -rf main.o
	rm -rf input.o
	rm -rf node.o

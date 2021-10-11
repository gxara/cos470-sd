
all:
	g++ -g -I. -c -o coordinator.o main.cpp
	g++ -lpthread coordinator.o -o main
clean:
	rm -f coordinator
	rm -f *.o
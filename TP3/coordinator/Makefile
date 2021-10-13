
all:
	g++ -pthread -std=c++11 main.cpp socketHandler.cpp parser.cpp -o  coordinator
	@./coordinator

compile:
	g++ -pthread -std=c++11 -c main.cpp
	g++ -pthread -std=c++11 -c socketHandler.cpp
	g++ -pthread -std=c++11 -c parser.cpp
	g++ -o main main.o socketHandler.o parser.o

clean:
	rm -f coordinator
	rm -f *.o
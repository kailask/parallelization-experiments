
all: litmus dekkers

litmus:
	g++ -std=c++17 relaxed.cpp -lpthread -o relaxed.out -O3
	g++ -std=c++17 consistent.cpp -lpthread -o consistent.out -O3

dekkers:
	g++ -std=c++17 -O3 -DSCDEKKERS  dekkers.cpp -lpthread -o scdekker.out
	g++ -std=c++17 -O3 -DRDEKKERS   dekkers.cpp -lpthread -o rdekkers.out
	g++ -std=c++17 -O3 -DTSODEKKERS dekkers.cpp -lpthread -o tsodekkers.out

clean:
	rm *.out
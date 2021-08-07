
all:
	g++ -std=c++17 blur-spawn.cpp -lpthread -o sjbarrier.out
	g++ -std=c++17 -DSRBARRIER blur-barrier.cpp -lpthread -o srbarrier.out
	g++ -std=c++17 -DSROBARRIER blur-barrier.cpp -lpthread -o srobarrier.out

clean:
	rm *.out

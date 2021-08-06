
all: mutex rw_mutex

test: mutex
	@./analyze.py ./cpp_mutex.out 8
	@./analyze.py ./filter_mutex.out 8
	@./analyze.py ./bakery_mutex.out 8

mutex:
	g++ -DCPP_MUTEX benchmark.cpp -lpthread -o cpp_mutex.out
	g++ -DFILTER_MUTEX benchmark.cpp -lpthread -o filter_mutex.out
	g++ -DBAKERY_MUTEX benchmark.cpp -lpthread -o bakery_mutex.out

rw_mutex:
	g++ -DDEFAULT_MUTEX benchmark-rw.cpp -lpthread -o rw_mutex.out
	g++ -DFAIR_MUTEX benchmark-rw.cpp -lpthread -o fair_rw_mutex.out

clean:
	rm *.out

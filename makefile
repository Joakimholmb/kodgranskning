# VARIABLES
CC = gcc
WFLAGS = -Wall -pedantic -g -pg
FLAGS = -Wall -pedantic -g -std=c11
TEST = -lcunit
VALTEST = valgrind --leak-check=full
TFLAGS = -fprofile-arcs -ftest-coverage

# BUILD FILES
test: hash_test list_test

run: hash_valgrind list_valgrind

# C. FILES
freq-count: freq-count.c hash_table.o list_linked.o iterator.o
	$(CC) $(WFLAGS) freq-count.c -o freq-count hash_table.o list_linked.o iterator.o

hash_table: hash_table.c list_linked.o
	$(CC) $(FLAGS) -c -o hash_table hash_table.c

list_linked: list_linked.c
	$(CC) $(FLAGS) -c -o list_linked list_linked.c

iterator: iterator.c
	$(CC) $(FLAGS) -c -o iterator iterator.c

# TESTFILES
hash_test: hash_test.c hash_table.o list_linked.o iterator.o
	$(CC) $(FLAGS) $(TFLAGS) -o $@ $< hash_table.o list_linked.o iterator.o $(TEST)
	#./$@

list_test: list_test.c list_linked.o iterator.o
	$(CC) $(FLAGS) $(TFLAGS) -o $@ $< list_linked.o iterator.o $(TEST)               
	#./$@

#nVALGRIND
hash_valgrind: hash_test
	$(VALTEST) ./hash_test

list_valgrind: list_test
	$(VALTEST) ./list_test

# OBJECTFILES
hash_table.o: hash_table.c hash_table.h common.h
	$(CC) $(FLAGS) -c $<

list_linked.o: list_linked.c list_linked.h 
	$(CC) $(FLAGS) -c $<

iterator.o: iterator.c iterator.h 
	$(CC) $(FLAGS) -c $<

# CLEAN 
clean:
	rm -f freq_count hash_table hash_test hash_valgrind list_linked iterator list_test list_valgrind
	rm -f *~

.PHONY: hash_test list_test clean

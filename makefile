CC	     = g++
FLAGS	 = -Wall
OMPFLAGS = -fopenmp
LFLAGS	 = -lgmp

PROJ           = test benchmark
OBJS_CORE      = utils/m7math.o scrypt/scrypt.o scrypt/scrypt-arm.o common/sha2.o common/blake2b.o power2b/yespower.o
OBJS_TEST      = $(OBJS_CORE) test.o
OBJS_BENCHMARK = $(OBJS_CORE) benchmark.o

all: $(PROJ)

benchmark: $(OBJS_BENCHMARK)
	$(CC) $(FLAGS) $(LFLAGS) $(OMPFLAGS) $(OBJS_BENCHMARK) -o benchmark.out

benchmark.o: benchmark.cpp
	$(CC) -c $(FLAGS) $(LFLAGS) $(OMPFLAGS) benchmark.cpp -o benchmark.o

test: $(OBJS_TEST)
	$(CC) $(FLAGS) $(LFLAGS) $(OBJS_TEST) -o test.out

test.o: test.cpp
	$(CC) -c $(FLAGS) $(LFLAGS) test.cpp -o test.o

utils/m7math.o: utils/m7math.cpp
	$(CC) -c $(FLAGS) utils/m7math.cpp -o utils/m7math.o

scrypt/scrypt.o: scrypt/scrypt.cpp
	$(CC) -c $(FLAGS) scrypt/scrypt.cpp -o scrypt/scrypt.o

scrypt/scrypt-arm.o: scrypt/scrypt-arm.S
	$(CC) -c $(FLAGS) scrypt/scrypt-arm.S -o scrypt/scrypt-arm.o

common/sha2.o: common/sha2.cpp
	$(CC) -c $(FLAGS) common/sha2.cpp -o common/sha2.o

common/blake2b.o: common/blake2b.c
	$(CC) -c $(FLAGS) common/blake2b.c -o common/blake2b.o

power2b/yespower.o: power2b/yespower.c
	$(CC) -c $(FLAGS) power2b/yespower.c -o power2b/yespower.o

clean:
	rm -f *.out $(OBJS_BENCHMARK) $(OBJS_TEST)

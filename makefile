OBJS	= test.o utils/m7math.o scrypt/scrypt.o scrypt/scrypt-arm.o common/sha2.o common/blake2b.o power2b/yespower.o
SOURCE	= test.cpp utils/m7math.cpp scrypt/scrypt.cpp scrypt/scrypt-arm.S common/sha2.cpp common/blake2b.c power2b/yespower.c
OUT	    = a.out
CC	    = g++
FLAGS	= -c -Wall
LFLAGS	= -lgmp

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

test.o: test.cpp
	$(CC) $(FLAGS) test.cpp -o test.o

utils/m7math.o: utils/m7math.cpp
	$(CC) $(FLAGS) utils/m7math.cpp -o utils/m7math.o

scrypt/scrypt.o: scrypt/scrypt.cpp
	$(CC) $(FLAGS) scrypt/scrypt.cpp -o scrypt/scrypt.o

scrypt/scrypt-arm.o: scrypt/scrypt-arm.S
	$(CC) $(FLAGS) scrypt/scrypt-arm.S -o scrypt/scrypt-arm.o

common/sha2.o: common/sha2.cpp
	$(CC) $(FLAGS) common/sha2.cpp -o common/sha2.o

common/blake2b.o: common/blake2b.c
	$(CC) $(FLAGS) common/blake2b.c -o common/blake2b.o

power2b/yespower.o: power2b/yespower.c
	$(CC) $(FLAGS) power2b/yespower.c -o power2b/yespower.o

clean:
	rm -f $(OBJS) $(OUT)

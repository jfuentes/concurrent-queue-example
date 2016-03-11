
CFLAGS = -g -Wall -Wextra -Iutil -Lutil

all: sol1 sol2

sol1: student_sol1.c
	$(CC) $(CFLAGS) student_sol1.cc -o student_sol2 -O3 -pthread -std=c++11

sol2: student_sol1.c
	$(CC) $(CFLAGS) student_sol2.cc -o student_sol2 -O3 -pthread -std=c++11

clean:
	rm -rf student_sol1 student_sol2 *.o *.dSYM *.greg

.PHONY: all clean test

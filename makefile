
CFLAGS = -g -Iutil -Lutil

all: student

student: student.cc
	g++ $(CFLAGS) student.cc -o student -O3 -pthread -std=c++11

clean:
	rm -rf student *.o

.PHONY: all clean test

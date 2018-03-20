
OBJS = dispatcher.o Node.o Process.o Queue.o

all: $(OBJS) sigtrap.c
	gcc -g $(OBJS) -o dispatcher -Wall
	gcc -g sigtrap.c -o process -Wall
dispatcher.o: dispatcher.c Queue.h
	gcc -Wall -g -c dispatcher.c
Node.o: Node.c Node.h Process.h
	gcc -Wall -g -c Node.c
Process.o: Process.c Process.h
	gcc -Wall -g -c Process.c
Queue.o: Queue.c Queue.h Node.h
	gcc -Wall -g -c Queue.c

clean:
	rm -rf *.dSYM
	rm dispatcher
	rm process
	rm -f *.o

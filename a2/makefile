CC=gcc
CFLAGS=-Wall -g
OBJECTS=test_manager.o pid_manager.o

test_manager: $(OBJECTS)
	$(CC) -o test_manager $(CFLAGS) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) -rf test_manager *.o *.dSYM/

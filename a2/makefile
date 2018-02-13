CC=gcc
CFLAGS=-Wall -g
MANAGER_OBJECTS=test_manager.o pid_manager.o
STATS_OBJECTS=thread_stats.o pid_manager.o

test_manager: $(MANAGER_OBJECTS)
	$(CC) -o test_manager $(CFLAGS) $(MANAGER_OBJECTS)

thread_stats: $(STATS_OBJECTS)
	$(CC) -o thread_stats $(CFLAGS) $(STATS_OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) -rf test_manager thread_stats *.o *.dSYM/

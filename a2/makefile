all: p320 p420 p421

p320: p320.c
	gcc -o p320 p320.c

p420: p420.c
	gcc p420.c -o p420 -lpthread

p421: p421.c
	gcc p421.c -o p421 -lpthread

clean:
	rm p421 p420 p320

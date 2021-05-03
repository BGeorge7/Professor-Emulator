all:	profemulator

profemulator:	profemulator.o
	gcc profemulator.o -o profemulator -lpthread

profemulator.o:	profemulator.c
	gcc -c profemulator.c -o profemulator.o -lpthread

clean:
	rm -f profemulator profemulator.o core *~


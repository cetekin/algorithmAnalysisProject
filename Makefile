all: main.o queue.o
	gcc main.o queue.o -o cikis
	@echo
	@echo
	@echo
	./cikis


main.o:
	gcc -c main.c

queue.o:
	gcc -c queue.c

clean:
	rm *.o
	rm cikis

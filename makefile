pscheduler: pscheduler.o main.o
	gcc -o pscheduler pscheduler.o main.o

main.o: pscheduler.o main.c
	gcc -c main.c

pscheduler.o: pscheduler.c pscheduler.h
	gcc -c pscheduler.c
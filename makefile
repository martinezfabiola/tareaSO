CC=gcc
CFLAGS=-I.
DEPS = pscheduler.h
OBJ = pscheduler.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

pscheduler: $(0BJ)
	gcc -o $@ $^ $(CFLAGS)
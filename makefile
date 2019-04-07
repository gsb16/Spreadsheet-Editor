
# GRR 20166812 -- Gabriel de Souza Barreto

CFLAGS = -Wall

myxcel: myxcel.c func.c func.h
	gcc myxcel.c func.c -lncurses -o myxcel $(CFLAGS)

purge: clean
	-rm myxcel 
#Pasta com as .h
IDIR=-I include

#Pasta com as .c
SDIR=src

#Compilador
CC=gcc

CFLAGS= -std=c99

# todas as minhas .c
_SRC=servidor.c projetil.c calculos.c

SRC=$(patsubst %,$(SDIR)/%,$(_SRC))

#nome do meu exe
EXECUTABLE=matapombocanhao

all:
	$(CC) -Wall $(CFLAGS) $(IDIR) $(SRC) -o $(EXECUTABLE) $(LIBS)

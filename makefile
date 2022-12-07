#
# makefile for TINY
# Borland C Version
# K. Louden 2/3/98
#

CC = gcc 

CFLAGS = -Wall -Wextra 

OBJS = main.o util.o lex.yy.o 

#tiny.exe: $(OBJS)
#	$(CC) $(CFLAGS) -etiny $(OBJS)

all: hw1_binary

hw1_binary : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -lfl

main.o: main.c globals.h util.h 
	$(CC) $(CFLAGS) -c -o $@ $<

util.o: util.c util.h globals.h
	$(CC) $(CFLAGS) -c util.c


#parse.obj: parse.c parse.h scan.h globals.h util.h
#	$(CC) $(CFLAGS) -c parse.c

#symtab.obj: symtab.c symtab.h
#	$(CC) $(CFLAGS) -c symtab.c

#analyze.obj: analyze.c globals.h symtab.h analyze.h
#	$(CC) $(CFLAGS) -c analyze.c

#code.obj: code.c code.h globals.h
#	$(CC) $(CFLAGS) -c code.c

#cgen.obj: cgen.c globals.h symtab.h code.h cgen.h
#	$(CC) $(CFLAGS) -c cgen.c

clean:
	rm -rf *.o  lex.yy.c *.gch *.exe *.txt
fclean : clean 
	rm -rf hw1_binary 
#tm.exe: tm.c
#	$(CC) $(CFLAGS) -etm tm.c

#tiny: tiny.exe
#tm: tm.exe

lex.yy.o: lex.yy.c globals.h util.h 
	$(CC) $(CFLAGS) -c -o $@ $<

lex.yy.c: cminus.l
	flex -o $@ $<

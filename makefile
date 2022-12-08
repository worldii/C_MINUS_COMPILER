#
# makefile for TINY
# Borland C Version
# K. Louden 2/3/98
#

CC = gcc 
CFLAGS=-c -Wall -O3 

SRCS=main.c util.c cminus.lex.c cminus.tab.c
OBJS=main.o util.o cminus.lex.o cminus.tab.o

all = hw2_binary
hw2_binary: cminus.lex.c cminus.tab.c $(OBJS)
	$(CC) -o $@ $(OBJS)

cminus.tab.c: cminus.lex.c cminus.y
	bison -o cminus.tab.c -vd cminus.y

cminus.lex.c: cminus.l
	flex -o cminus.lex.c cminus.l

clean:
	rm -f $(OBJS) cminus.lex.c cminus.tab.c cminus.tab.h cminus.output
fclean : clean 
	rm -rf hw2_binary test1_20182202.txt
CC=gcc
CFLAGS=-c -Wall -O3
EXEC= hw2_binary

.SUFFIXES: .c .o

LEXCODE=cminus.l
LEXSRC=cminus.lex.c

BISONCODE=cminus.y
BISONSRC=cminus.tab.c
BISONHDR=cminus.tab.h
BISONVERBOSE=cminus.output

SRCS=main.c util.c $(LEXSRC) $(BISONSRC)
OBJS=$(SRCS:.c=.o)

$(EXEC): $(LEXSRC) $(BISONSRC) $(OBJS)
	$(CC) -o $@ $(OBJS)

$(BISONSRC): $(LEXSRC) $(BISONCODE)
	bison -o $(BISONSRC) -vd $(BISONCODE)

$(LEXSRC): $(LEXCODE)
	flex -o $(LEXSRC) $(LEXCODE)

clean:
	rm -f $(OBJS) $(EXEC) $(LEXSRC) $(BISONSRC) $(BISONHDR) $(BISONVERBOSE)
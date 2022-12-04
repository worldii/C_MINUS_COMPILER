CC = gcc 

CFLAGS = -Wall -Wextra 

OBJS = main.o util.o lex.yy.o cminus.tab.o

TARGET = hw2_binary

all: $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -lfl

main.o: main.c globals.h util.h scan.h 
	$(CC) $(CFLAGS) -c -o $@ $<

util.o: util.c util.h globals.h
	$(CC) $(CFLAGS) -c util.c

lex.yy.o: lex.yy.c globals.h util.h 
	$(CC) $(CFLAGS) -c -o $@ $<
lex.yy.c: cminus.l
	flex -o $@ $<

cminus.tab.c cminus_tab.h : cminus.y
	bison -d cminus.y
cminus.tab.o : cminus.tab.c cminus.tab.h
	$(CC) $(CFLAGS) -c cminus.tab.c


## 명세서에 보면 -d 인자로 header file 생성할 수 있다고 나와 있음.
clean:
	rm -rf *.o  lex.yy.c *.gch *.exe *.txt cminus_tab.c cminus_tab.h 
fclean : clean 
	rm -rf hw2_binary 





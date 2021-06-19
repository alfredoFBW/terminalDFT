CC = gcc
CFLAGS = -Wall -g
LFLAGS = -g -lm

PROG = terminalDFT
OBJPROG = dft_main.o dft_show.o dft_calc.o
PREFIX = /usr/local

all: $(PROG)

$(PROG): $(OBJPROG)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	-rm -f $(PROG) *~ *.o

install: 
	cp $(PROG) $(PREFIX)/bin

uninstall:
	rm $(PREFIX)/bin/$(PROG)

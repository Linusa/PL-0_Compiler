CC=gcc
CFLAGS=-Wall

CDLIBS=
CDOBJS=runScanner.o runParser.o runVM.o compilerdriver.o
OF=programoutput.txt vmoutput.txt scanneroutput.txt tokenoutput.txt parseroutput.txt

cd: $(CDOBJS)
	$(CC) -o $@ $(CFLAGS) $(CDOBJS) $(CDLIBS)

runScanner.o: runScanner.c runScanner.h

runParser.o: runParser.c runParser.h

runVM.o: runVM.c runVM.h

compilerdriver.o: compilerdriver.c runScanner.h runParser.h runVM.h

clean:
	rm -vf cd $(CDOBJS) $(OF)

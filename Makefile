CC=g++
BINS= *.o
LIBS=-lX11
FLAGS=-Wall -Wextra

TESTMAIN=src/testmain.cc
OUTNAME=OApp
LIBNAME=libOKit.so
all: output

output: $(BINS)
	$(CC) $(FLAGS) -c $(TESTMAIN) $(LIBS)
	$(CC) $(FLAGS) $(BINS) -no-pie -o $(OUTNAME) $(LIBS)

shared: $(BINS)
	$(CC) -shared -fPIC $(FLAGS) $(BINS) -o $(LIBNAME) $(LIBS)

%.o: src/%.cpp
	$(CC) $(FLAGS) -fPIC -c $^ $(LIBS)
run:
	O_VERBOSE=1 ./$(OUTNAME)

clearShared:
	rm *.o *.so

clean:
	rm *.o $(OUTNAME)
clear:
	rm *.o $(OUTNAME)

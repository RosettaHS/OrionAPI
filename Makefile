CC=c++
BINS=*.o
LIBS=-lX11
FLAGS=-Wall -Wextra -Wpedantic

TESTMAIN=src/testmain.cc
OUTNAME=OApp
LIBNAME=libO.so

all: output

output: $(BINS)
	$(CC) $(FLAGS) -c $(TESTMAIN) $(LIBS)
	$(CC) $(FLAGS) $(BINS) -no-pie -o $(OUTNAME) $(LIBS)

shared:
	$(CC) $(FLAGS) -fPIC -O2 -c src/*.cpp $(LIBS)
	$(CC) $(FLAGS) -fPIC -O2 -c src/OSL/*.cpp $(LIBS)
	$(CC) $(FLAGS) -fPIC -O2 -c src/OKit/*.cpp $(LIBS)
	
	$(CC) $(FLAGS) -shared -fPIC -O2 $(BINS) -o $(LIBNAME) $(LIBS)
	strip --strip-all $(LIBNAME)

%.o: src/%.cpp src/OKit/%.cpp src/OSL/%.cpp
	$(CC) $(FLAGS) -fPIC -c $^ $(LIBS)

run:
	O_VERBOSE=1 ./$(OUTNAME)

clearShared:
	rm *.o *.so

clean:
	rm *.o $(OUTNAME)
clear:
	rm *.o $(OUTNAME)

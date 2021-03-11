CC=c++
BINS=*.o
LIBS=-lX11
FLAGS=-Wall -Wextra -Wpedantic

LIBNAME=libO.so

all: sharedMinimal

sharedMinimal:
	$(CC) $(FLAGS) -fPIC -c src/*.cpp $(LIBS)
	$(CC) $(FLAGS) -fPIC -c src/OSL/*.cpp $(LIBS)
	
	$(CC) $(FLAGS) -shared -fPIC $(BINS) -o $(LIBNAME) $(LIBS)

shared:
	$(CC) $(FLAGS) -fPIC -O2 -c src/*.cpp $(LIBS)
	$(CC) $(FLAGS) -fPIC -O2 -c src/OSL/*.cpp $(LIBS)
	
	$(CC) $(FLAGS) -shared -fPIC -O2 $(BINS) -o $(LIBNAME) $(LIBS)
	strip --strip-all $(LIBNAME)

clean:
	rm *.o *.so
clear:
	rm *.o *.so

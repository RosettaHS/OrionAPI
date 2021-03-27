CC=c++
BINS=*.o
OKITLIBS=-lxcb
FLAGS=-Wall -Wextra -Wpedantic

LIBNAME=libO.so

all: sharedMinimal

sharedMinimal:
	$(CC) $(FLAGS) -fPIC -c src/*.cpp
	$(CC) $(FLAGS) -fPIC -c src/OSL/*.cpp
	$(CC) $(FLAGS) -fPIC -c src/OKit/*.cpp $(OKITLIBS)
	
	$(CC) $(FLAGS) -shared -fPIC $(BINS) -o $(LIBNAME) $(LIBS)

shared:
	$(CC) $(FLAGS) -fPIC -O2 -c src/*.cpp
	$(CC) $(FLAGS) -fPIC -O2 -c src/OSL/*.cpp
	$(CC) $(FLAGS) -fPIC -c src/OKit/*.cpp $(OKITLIBS)
	
	$(CC) $(FLAGS) -shared -fPIC -O2 $(BINS) -o $(LIBNAME) $(LIBS)
	strip --strip-all $(LIBNAME)

clean:
	rm *.o *.so
clear:
	rm *.o *.so

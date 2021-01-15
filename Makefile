CC=c++
SRCS=src/*.cpp
BINS=CContainable.o CContainer.o CContext.o CNodeArray.o CSignalListener.o CEventArray.o CXEvent.o OCol.o OContainer.o OService.o OString.o OTheme.o OVec.o application.o xservice.o
ALLBINS=$(BINS) testmain.o
LIBS=-lX11
FLAGS=-Wall -Wextra -Wpedantic

TESTMAIN=src/testmain.cc
OUTNAME=OApp
LIBNAME=libOKit.so

all: output

output: $(BINS)
	$(CC) $(FLAGS) -c $(TESTMAIN) $(LIBS)
	$(CC) $(FLAGS) $(ALLBINS) -no-pie -o $(OUTNAME) $(LIBS)

shared: $(BINS)
	$(CC) -shared -fPIC $(FLAGS) $(BINS) -o $(LIBNAME) $(LIBS)

$(BINS): $(SRCS)
	$(CC) $(FLAGS) -fPIC -c $(SRCS) $(LIBS)
run:
	O_VERBOSE=1 ./$(OUTNAME)

clearShared:
	rm *.o *.so

clean:
	rm *.o $(OUTNAME)
clear:
	rm *.o $(OUTNAME)

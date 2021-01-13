CC=c++
CFLAGS=-Wall -Wextra -Wpedantic
SRCS=CContainable.cpp CContainer.cpp CContext.cpp CNodeArray.cpp CXEvent.cpp OCol.cpp OContainer.cpp OService.cpp OString.cpp OTheme.cpp OVec.cpp application.cpp xservice.cpp
BINS=CContainable.o CContainer.o CContext.o CNodeArray.o CXEvent.o OCol.o OContainer.o OService.o OString.o OTheme.o OVec.o application.o xservice.o
ALLBINS=$(BINS) testmain.o
LIBS=-lX11

TESTMAIN=testmain.cc
OUTNAME=OApp
LIBNAME=libOKit.so

all: output

output: $(BINS)
	$(CC) $(CFLAGS) -c $(TESTMAIN) $(LIBS)
	$(CC) $(CFLAGS) $(ALLBINS) -no-pie -o $(OUTNAME) $(LIBS)

shared: $(BINS)
	$(CC) -shared -fPIC $(CFLAGS) $(BINS) -o $(LIBNAME) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) -fPIC -c $< $(LIBS)
run:
	O_VERBOSE=1 ./$(OUTNAME)

clearShared:
	rm *.o *.so

clean:
	rm *.o $(OUTNAME)
clear:
	rm *.o $(OUTNAME)

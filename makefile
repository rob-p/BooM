# CC=/usr/bin/g++
CC=g++
CFLAGS = -O3 -std=c++11 -lpthread -flto -march=native -I include
LDFLAGS=-flto
EXEC=test
all: $(EXEC)

ifeq ($(prof),1)
 CFLAGS+= -pg
endif
ifeq ($(deb),1)
 CFLAGS+= -O0 -DASSERTS -g
endif


all: $(EXEC)

test:  src/test.cpp include/BooPHF.hpp include/BooMap.hpp
	$(CC)  $^ $(CFLAGS)

%.o: %.cpp %.h
	$(CC) -o $@ -c $< $(CFLAGS)


clean:
	rm -rf *.o
	rm test 

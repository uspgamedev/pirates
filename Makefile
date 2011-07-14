
OBJS = test.o
CXXFLAGS = -fPIC -O2 -I/usr/include/python2.6 -I/usr/include/panda3d

.PHONY: all
all: test

test.o: test.cc

test: ${OBJS}
	g++ ${OBJS} -o test -fPIC -L/usr/lib/panda3d -lp3framework -lpanda -lpandafx -lpandaexpress -lp3dtoolconfig -lp3pystub -lp3direct

.PHONY: clean
clean:
	rm *.o test


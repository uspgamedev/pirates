UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
# MacOSX
PANDA3DINCLUDE = /Developer/Panda3D/include
PANDA3DLIBS = /Developer/Panda3D/lib
else
PANDA3DINCLUDE = /usr/include/panda3d
PANDA3DLIBS = /usr/lib/panda3d
endif

SRCFOLDER = src
OBJFOLDER = obj
PYTHONINCLUDE = /usr/include/python2.6
OBJS = ${OBJFOLDER}/main.o
CXXFLAGS = -fPIC -O2 -I${PYTHONINCLUDE} -I${PANDA3DINCLUDE}

.PHONY: all
all: main

${OBJS}: ${SRCFOLDER}/main.cpp

main: ${OBJS}
	g++ ${OBJS} -o main -fPIC -L${PANDA3DLIBS} -lp3framework -lpanda -lpandafx -lpandaexpress -lp3dtoolconfig -lp3dtool -lp3pystub -lp3direct

.PHONY: clean
clean:
	rm ${OBJFOLDER}/* main


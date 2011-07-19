UNAME := $(shell uname)

PYTHONINCLUDE = /usr/include/python2.6
SRC_DIR = src
OBJ_DIR = .temp
OBJS = ${OBJ_DIR}/main.o ${OBJ_DIR}/game.o
OUTPUT_DIR = bin
OUTPUT = $(OUTPUT_DIR)/pirates

CXX = g++
CXXFLAGS = -fPIC -O2 -I${PYTHONINCLUDE} -I${PANDA3DINCLUDE}

ifeq ($(UNAME), Darwin)
# MacOSX
PANDA3DINCLUDE = /Developer/Panda3D/include
PANDA3DLIBS = /Developer/Panda3D/lib
CXXFLAGS += -arch i386
else
PANDA3DINCLUDE = /usr/include/panda3d
PANDA3DLIBS = /usr/lib/panda3d
endif


.PHONY: all
all: pirates

$(OBJ_DIR):
	mkdir $@

$(OUTPUT_DIR):
	mkdir $@

.temp/%.o: src/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

.temp/main.o: src/main.cpp src/game.h
.temp/game.o: src/game.cpp src/game.h

pirates: $(OBJ_DIR) $(OUTPUT_DIR) $(OBJS)
	g++ $(OBJS) -o $(OUTPUT) ${CXXFLAGS} -L${PANDA3DLIBS} -lp3framework -lpanda -lpandafx -lpandaexpress -lp3dtoolconfig -lp3dtool -lp3pystub -lp3direct

.PHONY: clean
clean:
	rm $(OUTPUT)
	rm -rf $(OBJ_DIR)


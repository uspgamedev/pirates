
SRC_DIR = src
OBJ_DIR = .temp
OBJS = .temp/main.o
OUTPUT_DIR = bin
OUTPUT = $(OUTPUT_DIR)/pirates

CXX = g++
CXXFLAGS = -fPIC -O2 -I/usr/include/python2.6 -I/usr/include/panda3d

.PHONY: all
all: pirates

$(OBJ_DIR):
	mkdir $@

$(OUTPUT_DIR):
	mkdir $@

.temp/%.o: src/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

.temp/main.o: src/main.cpp

pirates: $(OBJ_DIR) $(OUTPUT_DIR) $(OBJS)
	g++ $(OBJS) -o $(OUTPUT) -fPIC -L/usr/lib/panda3d -lp3framework -lpanda -lpandafx -lpandaexpress -lp3dtoolconfig -lp3pystub -lp3direct

.PHONY: clean
clean:
	rm $(OUTPUT)
	rm -rf $(OBJ_DIR)


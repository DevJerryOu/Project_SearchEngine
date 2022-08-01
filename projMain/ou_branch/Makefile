SRC=./src
INC=./include/
SOURCE:=$(wildcard $(SRC)/*.cc)
OUTS:=./bin/main
CC:=g++
INCLUDE:= -I $(INC)
LINKS:=-lpthread -lredis++ -lhiredis
COMPILE_ARGS:=-std=c++17
RUN:=./bin/./main
OPTION:=-D_GLIBCXX_USE_CXX11_ABI=0
${OUTS}:${SOURCE}
	$(CC) $^ -o $@ $(LINKS) $(COMPILE_ARGS) $(INCLUDE) 
	
.PHONY:clean

clean:
	rm $(OUTS)
run:
	$(OUTS)


SRC=./src
INC=./include/
SOURCE:=$(wildcard $(SRC)/*.cc)
OUTS:=./bin/main
CC:=g++
INCLUDE:= -I $(INC)
LINKS:=-lpthread -lredis++ -lhiredis
COMPILE_ARGS:=-std=c++17
RUN:=./bin/./main
${OUTS}:${SOURCE}
	$(CC) $^ -o $@ $(LINKS) $(COMPILE_ARGS) $(INCLUDE)
	
.PHONY:clean

clean:
	rm $(OUTS)
run:
	$(RUN)


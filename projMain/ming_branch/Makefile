SRC=./src
INC=./include/
SOURCE:=$(wildcard $(SRC)/*.cc)
OUTS:=./bin/main
CC:=g++
INCLUDE:= -I $(INC)
LINKS:=-lpthread
RUN:=./bin/./main

${OUTS}:${SOURCE}
	$(CC) $^ -o $@ $(LINKS) $(INCLUDE)
	
.PHONY:clean

clean:
	rm $(OUTS)
run:
	$(RUN)
SRC=./src
INC=./include/
SOURCE:=$(wildcard $(SRC)/*.cpp)
OUTS:=./bin/main
CC:=g++
INCLUDE:= -I $(INC)
${OUTS}:${SOURCE}
	$(CC) $^ -o $@ -lpthread $(INCLUDE)
.PHONY:clean
clean:
	rm $(OUTS)
OUT:=client
LINKS:=-lhiredis -lredis++
COMPILE_ARGS:=-std=c++17
RUN:=./$(OUT)
$(OUT): *.cc
	g++ $^ -o $(OUT) 

.PHONY:clean

clean:
	rm $(OUTS)
run:
	$(RUN)
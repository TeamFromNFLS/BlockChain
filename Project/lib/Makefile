CC = g++

OBJS = $(wildcard *.cpp)
INC = -I../include
TARGET = blockChain.dll
LIB = blockChain.lib
 
all:
	$(CC) $(INC) $(OBJS) -shared -o $(TARGET) -Wl,--out-implib,$(LIB)
clean:
	rm $(SRCS) $(headfile)
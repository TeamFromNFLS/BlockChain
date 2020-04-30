CC = g++

OBJS = $(wildcard *.cpp)
INC = -I../include
TARGET = blockChain.dll
LIB = blockChain.lib
SO = libChain.so
 
all:
	$(CC) $(INC) $(OBJS) -fPIC -shared -o $(SO) -pthread
clean:
	rm $(SRCS) $(headfile)
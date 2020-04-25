CC = g++

OBJS = $(wildcard *.cpp)

TARGET = blockChain.dll
LIB = blockChain.lib
 
all:
	$(CC) $(OBJS) -shared -o $(TARGET) -Wl,--out-implib,$(LIB) -pthread

clean:
	rm $(SRCS) $(headfile)
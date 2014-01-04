CC = g++
CFLAGS = -Wall -pedantic -std=c++11 -g -static -I/usr/include/bullet/
PROG = main

SRCS =  main.cpp Shader.cpp FatalException.cpp Logger.cpp shape.cpp quad.cpp
OBJS := $(SRCS:.cpp=.o)
LIBS = -lGL -lBulletDynamics -lBulletCollision -lLinearMath -lGLEW -lglfw

all: $(PROG)

$(OBJS): $(subst .o,.d,$@)
	$(CC) -MMD $(CFLAGS) -c $(subst .o,.cpp,$@) -o $@

$(PROG): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(PROG)

clean:
	rm -f $(PROG)


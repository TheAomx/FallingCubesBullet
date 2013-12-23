CC = g++
CFLAGS = -O0 -Wall -pedantic -std=gnu++0x
PROG = main

SRCS =  main.cpp Shader.cpp FatalException.cpp Logger.cpp shape.cpp quad.cpp
LIBS = -lGLEW -lglfw -lGL -lBulletDynamics -lBulletCollision -lLinearMath -L ./


all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)

CC = g++
CFLAGS = -Wall -pedantic -std=gnu++14 -g -static -I /usr/local/include/bullet/ -O2
PROG = main

SRCS =  main.cpp Shader.cpp FatalException.cpp Logger.cpp shape.cpp quad.cpp DynamicsWorld.cpp camera.cpp
OBJS := $(SRCS:.cpp=.o)
prereq_files := $(SRCS:.cpp=.d)
LIBS = -L ./ -lGL -lBulletDynamics -lBulletCollision -lLinearMath -lBulletSoftBody -lGLEW -lglfw -lm

all:

# All whitespace-separated words in the working directory and its subdirectories
# that do match any of the pattern words $(prereq_files). file names shall not
# contain the '%' character.
existant_prereqs := \
   $(filter $(prereq_files),$(shell find -regex '.*\.d$$' -printf '%P\n'))

# Was any goal (other than 'clean') specified on the command line? None counts
# as 'all'.
ifneq ($(filter-out clean,$(or $(MAKECMDGOALS),all)),)
   # Include existant makefiles of prerequisite . After reading in all those
   # files none of them will have to be updated. Non-existant prerequisite files
   # will be build along with their respective object files.
   include $(existant_prereqs)
endif

.PHONY: all

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(PROG)

clean:
	rm -f $(PROG) $(OBJS) $(prereq_files)

$(prereq_files):

.SECONDEXPANSION:

$(OBJS): $$(subst .o,.d,$$@)
	$(CC) -MMD $(CFLAGS) -c $(subst .o,.cpp,$@) -o $@


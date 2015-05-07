#
# Makefile for a CS 360 lab or programming assignment.
# Do not modify this file!!!
#

CXXFLAGS = -g -Wall

SRC = matvec_par.c
OBJ = $(SRC:.c=.o)
EXE = $(SRC:.c=)

$(EXE) : clean $(OBJ)
	$(CXX) -o $@ $(OBJ) -lm -lpthread

# This is just a dummy target to force other targets
# to always be out of date.
nonexistent :

# Remove generated files.
clean :
	rm -f *.o *.exe

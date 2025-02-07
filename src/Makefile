#------------------------------------------------------------------------------
# File:   Makefile
# Author: Guillermo Bajo Laborda (NIP: 842748)  
#------------------------------------------------------------------------------

#---------------------------------------------------------
# Macro definitions
CC = g++                 # Compiler
CPPFLAGS = -std=c++11    # Compilation options
LDFLAGS =                # Linking options
RM = rm -f               # Command to remove files

all: mainB

#-----------------------------------------------------------
# Compilation
mainB.o: mainB.cpp betris.hpp
	${CC} mainB.cpp -c ${CPPFLAGS}

# Linking
betris.o: betris.cpp betris.hpp
	${CC} betris.cpp -c ${CPPFLAGS}

mainB: mainB.o betris.o
	${CC} mainB.o betris.o -o mainB ${CPPFLAGS}

#-----------------------------------------------------------
# CLEANING all generated files. Note: They will be deleted without asking
clean:
	${RM} mainB.o betris.o

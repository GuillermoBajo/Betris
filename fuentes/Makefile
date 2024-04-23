#------------------------------------------------------------------------------
# File:   Makefile
# Author: Guillermo Bajo Laborda (NIP: 842748)  
#------------------------------------------------------------------------------

#---------------------------------------------------------
# Definición de macros
CC = g++
CPPFLAGS = -std=c++11      #opciones de compilación
LDFLAGS =                      #opciones de linkado
RM = rm -f                     #comando para borrar ficheros

all: mainB
#-----------------------------------------------------------
# Compilacion
mainB.o: mainB.cpp betris.hpp
	${CC} mainB.cpp -c ${CPPFLAGS}
# Linkado
betris.o: betris.cpp betris.hpp
	${CC} betris.cpp -c ${CPPFLAGS}
mainB: mainB.o betris.o
	${CC} mainB.o betris.o -o mainB ${CPPFLAGS}
#-----------------------------------------------------------	
# LIMPIEZA de todo lo generado. Ojo: se borrarán sin preguntar
clean:
	${RM} mainB.o betris.o

#include <iostream>
#include "betris.hpp"

using namespace std;


int main(int argc, char*argv[]){

    tpTablero tablero;
    int vEntrada[MAXENTRADA] = {};
    int objetivo;
    int retardo = 0;
    int vSalida[MAXENTRADA] = {};

    // Procesamos los datos introducidos por el usuario
    entradaDatos(argc, argv, objetivo, retardo, vEntrada, tablero);
    
    // Inicializamos el tablero base del juego (todas las casillas del betris vacías)
    inicializarTablero(tablero);

    // La primera llamada a buscaSolución se realiza con n = 0 (con la primera pieza introducida)
    int n = 0;
    int x = buscaSolucion(tablero, vEntrada, vSalida, objetivo, n, retardo);

    // Si se ha encontrado la solución mostramos el tablero como parte de la salida de datos
    system("clear");
    if(esSolucion(tablero, objetivo)){
        mostrarTablero(tablero, vEntrada);
    }

    // Mostramos los resultados obtenidos en el juego
    salidaDatos(vEntrada, vSalida, x, objetivo);

}


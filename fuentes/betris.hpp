#include <iostream>

using namespace std;

// CONSTANTES Y TIPOS

// Máximo múmero de filas y columnas del tablero
const int MAXDIM = 100;

// Máximo número de piezas en la entrada
const int MAXENTRADA = 3000;

// Tamaño en número de cuadrados de cada pieza
const int TAMPIEZA = 4;

// Número de piezas definidas
const int PIEZASDEF = 5;

// Códigos numéricos de colores
const int NEGRO    = 40;
const int ROJO     = 41;
const int VERDE    = 42;
const int AMARILLO = 43;
const int AZUL     = 44;
const int MAGENTA  = 45;
const int CIAN     = 46;
const int BLANCO   = 47;

// Tipo tablero
struct tpTablero{
    int nfils; // Número de filas del tablero
    int ncols; // Número de columnas del tablero
    int matriz[MAXDIM][MAXDIM]; // El valor de cada celda indica el numero de
                                // la pieza de entrada que la ocupa.
                                // Si el valor es -1 entonces la casilla está vacía.
                                // Las coordenadas de la casilla de arriba a la izquierda son (0,0)
};


// Tipo pieza (color y forma)
struct tpPieza{
   int color;              // Código de color de la pieza
   int forma[TAMPIEZA][2]; // Coordenadas de los cuadrados de la pieza
                           // El primer valor es el número de fila, el segundo es el número de columna
                           // La coordenada de arriba a la izquierda es (0,0)              
     // Ejemplo de forma: [[0,0], [0,1], [1,0], [2,0] ] es la pieza  XX 
     //                                                              X 
     //                                                              X
};

// Definición de piezas existentes. El número de cada pieza es su posición en este vector
const tpPieza vPiezas[PIEZASDEF] = {
    {AZUL,     {{0,0}, {0,1}, {0,2}, {0,3}}}, // 0  XXXX
    {VERDE,    {{0,0}, {1,0}, {2,0}, {3,0}}}, // 1  X
                                              //    X
                                              //    X
                                              //    X
    {ROJO,     {{0,0}, {0,1}, {1,0}, {2,0}}}, // 2  XX
                                              //    X
                                              //    X
    {CIAN,     {{0,0}, {0,1}, {0,2}, {1,2}}}, // 3  XXX
                                              //      X
    {AMARILLO, {{0,0}, {0,1}, {1,0}, {1,1}}}  // 4  XX
                                              //    XX
};


// FUNCIONES

// Pre: retardo es la pausa temporal a realizar después de mostrar el estado del tablero. 
//      mostrarTablero muestra el tablero en su estado actual
// Post: Si retraso > 0 se ha mostrado el tablero con su respectivo retraso. Sino, solo se ha mostrado
void mostrarConRetraso(const int retardo, tpTablero &tablero, const int vEntrada[MAXENTRADA]);

// Pre: 0 <= n <= MAXENTRADA
// Post: Ha generado una secuencia aleatoria de tamaño -n piezas y ha sido almacenada en vEntrada
void generarSecAleatoria(int& n, int vEntrada[MAXENTRADA]);

// Pre: True
// Post: Se almacenan los datos introducidos por el usuario en la terminal en las respectivas variables
void entradaDatos(int argc, char*argv[], int& objetivo, int& retardo, int vEntrada[MAXENTRADA], tpTablero &tablero);

// Pre: true
// Post: Todas las componentes de tablero.matriz son -1 (casilla vacía)
void inicializarTablero(tpTablero &tablero);

// Pre: vEntrada contiene los números de las piezas que entran y tablero contiene el estado actual 
//      del tablero, en el que pueden estar colocadas algunas de dichas piezas 
// Post: Se ha mostrado el tablero por pantalla (cada pieza se ha dibujado con su color)
void mostrarTablero(const tpTablero &tablero, const int vEntrada[MAXENTRADA]);

// Pre: tablero es el estado actual del tablero y objetivo es el número de filas objetivo a completar
// Post: Se ha devuelto false si el estado actual del tablero no es el final, y se ha devuelto true
//       en caso de que lo sea y por tanto se han completado objetivo filas
bool esSolucion(const tpTablero tablero, const int objetivo);

// Pre: n indica la posición de vEntrada en la que se encuentra la pieza que se quiere comprobar si es
//      colocable en la columna "columna"
// Post: En caso de poder colocar la pieza, devuelve la fila en la que se colocará, en caso contrario 
//       devuelve -1
int esColocable(tpTablero &tablero, const int vEntrada[MAXENTRADA], const int n, const int columna);

// Pre: 0 < fila < tablero.nfils ∧ 0 < columna < tablero.ncols ∧ 0 <= n <= MAXENTRADA
// Post: Se ha colocado la pieza en la matriz del tablero
void colocarPieza(tpTablero &tablero, const int vEntrada[MAXENTRADA],const int n, const int columna, const int fila);

// Pre: 0 < fila < tablero.nfils ∧ 0 < columna < tablero.ncols ∧ 0 <= n <= MAXENTRADA
// Post: Se ha retirado la pieza de la matriz del tablero
void quitarPieza(tpTablero &tablero, const int vEntrada[MAXENTRADA], const int n, const int columna, const int fila);

// Pre: vSalida contiene la columna asignada a cada pieza en el mismo orden en el que están en vEntrada, n es el total de 
//      piezas colocadas
// Post: Se han mostrado por pantalla los resultadso del juego (piezas de entrada, columnas asignadas a ellas y número de 
//       piezas colocadas)
void salidaDatos(const int vEntrada[MAXENTRADA], const int vSalida[MAXENTRADA], const int n, const int objetivo);

// Pre: En el tablero se han colocada las n primeras piezas de vEntrada, en la columnas indicadas respectivamente en vSalida
// Post: Si las piezas colocadas completan al menos objetivo filas sin huecos,
//       entonces
//           devuelve el número de piezas colocadas, en vSalida las columnas en las que se han colocado
//           las piezas y el tablero con las piezas colocadas
//       si no devuelve -1
int buscaSolucion(tpTablero &tablero, const int vEntrada[MAXENTRADA], int vSalida[MAXENTRADA], const int objetivo, int n, const int retardo=0);

//////////////////////////////////////////////////////////////////////////////
// Salida con atributos
//
// La cadena "<ESC>[{attr1};...;{attrn}m" sirve para asignar atributos a la salida
//
// Por ejemplo: 
//    cout << "\033[34;42m" << "Hola" << "\033[0m";
// escribe en azul con fondo verde, y después resetea los atributos

// Lista de atributos est   ándar:
// 0 Resetea todos los atributos
// 1 brillante
// 2 tenue
// 4 subrayado
// 5 parpadeo
// 7 invertido
// 8 oculto

// Colores de primer plano
// 30 negro
// 31 rojo
// 32 verde
// 33 amarillo
// 34 azul
// 35 magenta
// 36 cian
// 37 blanco

// Colores de fondo
// 40 negro
// 41 rojo
// 42 verde
// 43 amarillo
// 44 azul
// 45 magenta
// 46 cian
// 47 blanco

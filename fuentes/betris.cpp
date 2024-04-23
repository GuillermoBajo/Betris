    #include <iostream>
    #include <unistd.h>
    #include <time.h>
    #include <random>
    #include <ctime>
    #include <cstdlib>
    #include "betris.hpp"

    using namespace std;


    // Pre: retardo es la pausa temporal a realizar después de mostrar el estado del tablero. 
    //      mostrarTablero muestra el tablero en su estado actual
    // Post: Si retraso > 0 se ha mostrado el tablero con su respectivo retraso. Sino, solo se ha mostrado
    void mostrarConRetraso(const int retardo, tpTablero &tablero, const int vEntrada[MAXENTRADA]){

        if(retardo > 0){
            mostrarTablero(tablero, vEntrada);
            usleep(retardo* 1000);
        }
        // Si retardo <= 0, lo ignoramos
        else{
            mostrarTablero(tablero, vEntrada);
        }
    }


    // Pre: 0 <= n <= MAXENTRADA
    // Post: Ha generado una secuencia aleatoria de tamaño -n piezas y ha sido almacenada en vEntrada
    void generarSecAleatoria(int& n, int vEntrada[MAXENTRADA]){

        n = -n;
        time_t semilla = time(nullptr); 
        srand(semilla); 
        
        int i = 0;
        // Asignamos a vEntrada piezas creadas de forma aleatoria
        while(i < n){
            vEntrada[i] = 0 + rand()%(PIEZASDEF);
            i++;
        }
        // Establecemos el fin de las piezas
        vEntrada[i] = -1;
    }

    // Pre: True
    // Post: Se almacenan los datos introducidos por el usuario en la terminal en las respectivas variables
    void entradaDatos(int argc, char*argv[], int& objetivo, int& retardo, int vEntrada[MAXENTRADA], tpTablero &tablero){

        //Ignoramos argv[0] ya que contiene información irrelevante 
        tablero.nfils = atoi(argv[1]);
        tablero.ncols = atoi(argv[2]);
        objetivo = atoi(argv[3]);
        retardo = atoi(argv[4]);
        int indicePieza = 0;

        // Si N1 <0 se genera una secuencia aleatoria de -N1 piezas
        if(atoi(argv[5]) < 0){
            int p = atoi(argv[5]);
            generarSecAleatoria(p, vEntrada);
        }

        else{
            int i = 5;
            // Leemos las piezas introducidas y las asignamos a vEntrada
            while(i < argc -1){
                vEntrada[indicePieza] = atoi(argv[i]);
                indicePieza++;
                i++;
            }
            // Establecemos el fin de las piezas
            vEntrada[indicePieza] = -1;
        }
    }


    // Pre: True
    // Post: Todas las componentes de tablero.matriz son -1 (casilla vacía)
    void inicializarTablero(tpTablero &tablero){
        for (int i = 0; i < tablero.nfils; i++){
            for (int j = 0; j < tablero.ncols; j++){

                tablero.matriz[i][j] = -1;
            }
        }
    }


    // Pre: vEntrada contiene los números de las piezas que entran y tablero contiene el estado actual 
    //      del tablero, en el que pueden estar colocadas algunas de dichas piezas 
    // Post: Se ha mostrado el tablero por pantalla (cada pieza se ha dibujado con su color)
    void mostrarTablero(const tpTablero &tablero, const int vEntrada[MAXENTRADA]){
        system("clear");
        int colorP;
        int n;
        for (int i=0; i < tablero.nfils; i++){

            for(int j=0; j < tablero.ncols; j++){
            
                // Si la casilla del tablero está vacía no la pintamos
                if(tablero.matriz[i][j] == -1){
                    cout <<  " ";
                }
                // Si tiene un valor válido la pintamos del color correspondiente
                else{
                    n = vEntrada[tablero.matriz[i][j]]; 
                    colorP = vPiezas[n].color;
                    cout << "\033["  << colorP <<  "m" << " " << "\033[0m";
                }
            }
            cout << endl;
        }
    }


    // Pre: tablero es el estado actual del tablero y objetivo es el número de filas objetivo a completar
    // Post: Se ha devuelto false si el estado actual del tablero no es el final, y se ha devuelto true
    //       en caso de que lo sea y por tanto se han completado objetivo filas
    bool esSolucion(const tpTablero tablero, const int objetivo){
        
        int completadas=0; // Total de filas completadas
        int cont=0;        // Total de casillas rellenadas en una fila
        bool aunPosible;   // Variable para comprobar si la fila que se está leyendo aún puede ser completada
        int j;

        for(int i=0; i < tablero.nfils; i++){
            // Reiniciamos las variables necesarias
            j = 0;
            aunPosible = true;

            
            while(j < tablero.ncols && aunPosible){
                // Si la casilla leída está vacía salimos del bucle y leemos la siguiente fila
                if(tablero.matriz[i][j]==-1){
                    aunPosible = false;
                }
                // Sino, contamos una casilla más
                else{
                    cont++; 
                }

                j++;
            }
            // Al acabar de leer una fila, si el contador equivale a el total de columnas, hemos completado una fila
            if(cont == tablero.ncols){
                completadas++;
            }
            // Al pasar a la siguiente fila reiniciamos el contador de casillas
            cont=0;
            
        }
        // Devolvemos true si y solo si se han completado al menos objetivo filas
        return(completadas >= objetivo);
    }


    // Pre: n indica la posición de vEntrada en la que se encuentra la pieza que se quiere comprobar si es
    //      colocable en la columna "columna"
    // Post: En caso de poder colocar la pieza, devuelve la fila en la que se colocará, en caso contrario 
    //       devuelve -1
    int esColocable(tpTablero &tablero, const int vEntrada[MAXENTRADA], const int n, const int columna){

        int fila = -1; // Partimos de fila = -1, si no se modifica su valor durante la función, la pieza no será colocable
        bool aunColocable = true;
        tpPieza piezaComprobada = vPiezas[vEntrada[n]];
        int j;

        for(int i = 0; i < tablero.nfils; i++){
            // Reiniciamos las variables necesarias
            j = 0;
            while(j < TAMPIEZA && aunColocable){

                int a = piezaComprobada.forma[j][0];
                int b = piezaComprobada.forma[j][1];

                // Comprobamos si las coordenadas a revisar están ya ocupadas
                bool yaOcupada = tablero.matriz[i + a][columna + b] != -1;
                // Comprobamos que la pieza no se esté colocando fuera de los límites del tablero
                bool limites = ((columna + b >= tablero.ncols) || (i + a >= tablero.nfils));

                // Si se incumple una de las condiciones para colocar la pieza, ya no es colocable
                if(yaOcupada || limites){

                    aunColocable = false;
                }
                j++;
            }
            // Al acabar el bucle principal, se le asignará a la variable fila la fila donde se empezará
            // a construir la pieza (en caso de que sea colocable)
            if(aunColocable){
                fila = i;
            }
        }
        return fila;
    }


    // Pre: 0 < fila < tablero.nfils ∧ 0 < columna < tablero.ncols ∧ 0 <= n <= MAXENTRADA
    // Post: Se ha colocado la pieza en la matriz del tablero
    void colocarPieza(tpTablero &tablero, const int vEntrada[MAXENTRADA],const int n, const int columna, const int fila){
        
        tpPieza piezaAcolocar = vPiezas[vEntrada[n]];

        for(int i = 0; i < TAMPIEZA; i++){
                
            int a = piezaAcolocar.forma[i][0];
            int b = piezaAcolocar.forma[i][1];
            // Para colocar la pieza sumamos a fila la coordenada x de la pieza y a columna la coordenada y
            tablero.matriz[fila + a][columna +  b] = n;
        }
    }


    // Pre: 0 < fila < tablero.nfils ∧ 0 < columna < tablero.ncols ∧ 0 <= n <= MAXENTRADA
    // Post: Se ha retirado la pieza de la matriz del tablero
    void quitarPieza(tpTablero &tablero, const int vEntrada[MAXENTRADA], const int n, const int columna, const int fila){

        tpPieza piezaAcolocar = vPiezas[vEntrada[n]];

        for(int i = 0; i < TAMPIEZA; i++){
                
            int a = piezaAcolocar.forma[i][0];
            int b = piezaAcolocar.forma[i][1];
            // Para colocar la pieza sumamos a fila la coordenada x de la pieza y a columna la coordenada y
            tablero.matriz[fila + a][columna + b] = -1;
        }
    }


    // Pre: vSalida contiene la columna asignada a cada pieza en el mismo orden en el que están en vEntrada, n es el total de 
    //      piezas colocadas
    // Post: Se han mostrado por pantalla los resultadso del juego (piezas de entrada, columnas asignadas a ellas y número de 
    //       piezas colocadas)
    void salidaDatos(const int vEntrada[MAXENTRADA], const int vSalida[MAXENTRADA], const int n, const int objetivo){

        // Mostramos por pantalla las piezas de entrada
        cout << "Entrada (numeros de piezas) : ";
        int i = 0;
        while(vEntrada[i] != -1){
            cout << vEntrada[i] << " ";
            i++;
        }
        cout << endl;

        // Mostramos el total de piezas colocadas (-1 en caso de solución no encontrada)
        cout << "Numero de piezas colocadas: " << n << endl;

        // Mostramos las columnas asignadas a cada una de las piezas colocadas
        cout << "Salida (numeros de columnas) : ";
        for(int j = 0; j < n; j++){

            cout << vSalida[j] << " ";
        }
        cout << endl;

    }


    // Pre: En el tablero se han colocada las n primeras piezas de vEntrada, en la columnas indicadas respectivamente en vSalida
    // Post: Si las piezas colocadas completan al menos objetivo filas sin huecos,
    //       entonces
    //       devuelve el número de pi ezas colocadas, en vSalida las columnas en las que se han colocado
    //           las piezas y el tablero con las piezas colocadas
    //       si no devuelve -1
    int buscaSolucion(tpTablero &tablero, const int vEntrada[MAXENTRADA], int vSalida[MAXENTRADA], const int objetivo, int n, const int retardo){

        int tot = -1;  // En caso de no haber encontrado solución, esta variable no se modificará
        int fila;

        // Caso base
        if(esSolucion(tablero, objetivo)){
            return n;
        }

        else{

            int columna = 0;
            bool resuelto = false;

            while(columna < tablero.ncols && !resuelto){
                
                // Comprobamos si vEntrada[n] es colocable, y en ese caso le asginamos una fila
                fila = esColocable(tablero, vEntrada, n, columna);

                if(fila != -1){
                    // Si es colocable, la colocamos, mostramos el tablero y actualizamos vSalida
                    colocarPieza(tablero, vEntrada, n, columna, fila);
                    mostrarConRetraso(retardo, tablero, vEntrada);
                    vSalida[n]=columna;

                    // Utilizamos la recursividad para llamar a la siguiente pieza
                    tot = buscaSolucion(tablero, vEntrada, vSalida, objetivo, n+1, retardo);

                    // En caso de no haber encontrado solución se retira la pieza de la llamada correspondiente
                    if(tot == -1){
                        quitarPieza(tablero, vEntrada, n, columna, fila);
                    }
                    // Sino, salimos del bucle y pasamos el valor de tot desde la llamada actual hasta la primera llamada de todas
                    else{
                        resuelto = true;
                    }        
                }  
                columna++;
            }
        }
        return tot;
    }
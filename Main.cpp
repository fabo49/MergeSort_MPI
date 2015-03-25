#include <stdio.h>
#include <fstream>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string>
#include "mpi.h"

using namespace std;

int main(int argc, char **argv)
{

    MPI_Init(&argc, &argv);                                               // Arranque del proceso MPI


    int arrayDesorden[128];                                              // Array que almacena todos los numeros en desorden
    string numeros = "";
    int valor = 0;
    srand(time(NULL));

    ofstream numerosDesorden;                                           // Objeto Archivo

    numerosDesorden.open("ListaI.txt");                                 // Se crea el archivo que contiene los numeros en desorden
    numerosDesorden << "Números en desorden: " << endl;

    if(my_rank==0){                                                      // Proceso raiz 0

        for(int i = 0; i < 128; ++i){                                   // Se generan todos los numeros random 
            valor = (i* (rand()%10+1) );
            arrayDesorden[i] = valor;
            numeros = to_string(valor);
            numerosDesorden << numeros+" " << endl;                     // Se llena el archivo con valores random
        }
        
        MPI_Bcast(array, 16, MPI_INT, 0, MPI_COMM_WORLD);             // Encargado de entregar a cada proceso una cierta cantidad equitativa de elementos para que los ordene


    }else{
        
        


    }
    MPI_Finalize();                                                     // Finalización del proceso MPI
    return 0;
}
















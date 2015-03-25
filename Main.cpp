#include <stdio.h>
#include <fstream>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string>
//#include "mpi.h"

using namespace std;

int main(int argc, char **argv)
{

    MPI_Init(&argc, &argv);                                               // Arranque del proceso MPI


    int arrayDesorden[128];                                              // Array que almacena los numeros en desorden
    string numeros = "";
    int valor = 0;
    srand(time(NULL));

    ofstream numerosDesorden;                                           // Objeto Archivo

    numerosDesorden.open("ListaI.txt");                                 // Almacena los numeros en desorden
    numerosDesorden << "Números en desorden: " << endl;

    if(my_rank==0){                                                      // Proceso raiz

        for(int i = 0; i < 128; ++i){
            valor = (i* (rand()%10+1) );
            arrayDesorden[i] = valor;
            numeros = to_string(valor);
            numerosDesorden << numeros+" " << endl;                     // Se llena la lista desordenada con valores random
        }
        
        for(int j = 1; j < 9; ++j){
            
        }


    }else{


    }


    MPI_Finalize();                                                     // Finalización del proceso MPI
    return 0;
}


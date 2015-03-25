#include <stdio.h>
#include <fstream>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string>
#include "mpi.h"
#include "ordenador.h"

using namespace std;

int main(int argc, char **argv)
{

    int numProcesos;
    int numElementos;
    bool compatibles = false;
    while(compatibles == false){
        cout<<"Ingrese la cantidad de elementos que quiere en el vector: ";
        cin>>numElementos;
        cout<<"\nIngrese la cantidad de procesos que quiere crear para correr el programa: ";
        cin>>numProcesos;
        cout<<endl;
        if(numElementos % numProcesos == 0){
            compatibles = true;
        }else{
            cout<<"Los datos ingresados no funcionan, la cantidad de elementos tiene que ser"<<endl;
            cout<<"divisible entre la cantidad de procesos. Vuelva a ingresar los datos"<<endl;
            cout<<"------------------------------------------------------------------------"<<endl;
        }
    }

    ordenador metodoOrde;
    MPI_Init(&argc, &argv);                                               // Arranque del proceso MPI


    int arrayDesorden[128];                                              // Array que almacena todos los numeros en desorden
    int arrayTemporal[128];                                              // Array que usa cada proceso para guardar los numeros que la raiz envia
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
        
        MPI_Bcast(arrayDesorden, 16, MPI_INT, 0, MPI_COMM_WORLD);             // Encargado de entregar a cada proceso una cierta cantidad equitativa de elementos para que los ordene


    }else{
        
        MPI_Recv(arrayTemporal, 16, MPI_INT, 0, 911, MPI_COMM_WORLD, &status);      // Cada proceso recibe de la raiz los numeros iniciales a ordenar
        //MPI_Recv(arrayTemporal, 16, MPI_INT, 0, ANI
        metodoOrde.mergeSort(arrayTemporal, 16);                             // Envio a ordenar lo que me corresponde a mi


    }
    MPI_Finalize();                                                     // Finalización del proceso MPI
    return 0;
}
















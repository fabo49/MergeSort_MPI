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

    int numProcesos;            /* Va a guardar la cantidad de procesos que el usuario quiere ejecutar. */
    int numElementos;           /* Va a guardar la cantidad de elementos que el usuario quiere que tenga el vector*/
    bool compatibles = false;
    int tamVecTmp;              /* Cantidad de elementos que se destinan a cada proceso

    //ingreso de datos y comprobacion de compatibilidad
    while(compatibles == false){
        cout<<"Ingrese la cantidad de elementos que quiere en el vector: ";
        cin>>numElementos;
        cout<<"\nIngrese la cantidad de procesos que quiere crear para correr el programa: ";
        cin>>numProcesos;
        cout<<endl;
        if( (numElementos % numProcesos == 0) && (numProcesos <= 32) ){
            compatibles = true;
        }else{
            cout<<"Los datos ingresados no funcionan, la cantidad de elementos tiene que ser"<<endl;
            cout<<"divisible entre la cantidad de procesos. Vuelva a ingresar los datos"<<endl;
            cout<<"------------------------------------------------------------------------"<<endl;
        }
    }

    ordenador metodoOrde;
    MPI_Init(&argc, &argv);                                               // Arranque del proceso MPI
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesos);                         /* El comunicador MPI_COMM_WORLD almacena el numero de procesos a ejecutar en la 
                                                                            variable numProcesos */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);                             /* El comunicador MPI_COMM_WORLD almacena en la variable my_rank la etiqueta del 
                                                                           proceso que realiza el llamado. */

    if(my_rank==0){                                                      // Proceso raiz 0
    
        int arrayDesorden[numElementos];                                              // Array que almacena todos los numeros en desorden
        string numeros = "";
        int valor = 0;
        srand(time(NULL));

        ofstream numerosDesorden;                                           // Objeto Archivo

        numerosDesorden.open("ListaI.txt");                                 // Se crea el archivo que contiene los numeros en desorden
        numerosDesorden << "Números en desorden: " << endl;

        for(int i = 0; i < numElementos; ++i){                                   // Se generan todos los numeros random
            valor = (i* (rand()%10+1) );
            arrayDesorden[i] = valor;
            numeros = to_string(valor);
            numerosDesorden << numeros+" " << endl;                     // Se llena el archivo con valores random
        }

        
    }else{

        metodoOrde.mergeSort(arrayTemporal, 16);                             // Envio a ordenar lo que me corresponde a mi


    }
    MPI_Finalize();                                                     // Finalización del proceso MPI
    return 0;
}
















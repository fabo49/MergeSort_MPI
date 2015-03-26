/**
  * Universidad de Costa Rica
  * Escuela de Ciencias de la Computacion e Informatica
  * Arquitectura de Computadoras
  * Tarea programada 1
  * @author Fabian Rodriguez B25695
  * @author Jose Pablo Ureña B16692
  * I Semestre 2015
  * El programa implementa el algoritmo de merge sort con paralelismo y utilizando MPI para el paso de mensajes
  */

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
    int tamVecTmp;              /* Cantidad de elementos que se destinan a cada proceso */

    //ingreso de datos y comprobacion de compatibilidad
    while(compatibles == false){
        cout<<"Ingrese la cantidad de elementos que quiere en el vector: ";
        cin>>numElementos;
        cout<<"\nIngrese la cantidad de procesos que quiere crear para correr el programa: ";
        cin>>numProcesos;
        cout<<endl;
        if( (numElementos % numProcesos == 0) && (numProcesos <= 32) && (numProcesos > 0) && (numElementos > 0) ){
            compatibles = true;
        }else{
            cout<<"Los datos ingresados no funcionan, la cantidad de elementos tiene que ser"<<endl;
            cout<<"divisible entre la cantidad de procesos. Vuelva a ingresar los datos"<<endl;
            cout<<"------------------------------------------------------------------------"<<endl;
        }
    }

    ordenador metodoOrde;

    tamVecTmp = numElementos/numProcesos;               /* Cuantos elementos le tocan a cada proceso */
    int* vecTemporal;                                   /* Array de cada proceso */
    int* arrayDesorden;                                 /* Array que almacena todos los numeros en desorden*/
    int iteracion;                                      /* Para indicar en cual iteracion (o nivel del arbol) se encuentra */
    int* vecMerge;                                      /* En este array se van a acomodar los numeros de cada proceso mas los que recibe del otro. */
    int cantActualProcs;                                /* La cantidad de procesos que le tocan a cada subproceso. */
    MPI_Status estado;                                  /* Para saber cual es el estado del proceso. */

    MPI_Init(&argc, &argv);                                               /* Arranque del proceso MPI */

    MPI_Comm_size(MPI_COMM_WORLD, &numProcesos);                         /* El comunicador MPI_COMM_WORLD almacena el numero de procesos a ejecutar en la
                                                                            variable numProcesos */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);                             /* El comunicador MPI_COMM_WORLD almacena en la variable my_rank la etiqueta del
                                                                           proceso que realiza el llamado. */

    if(my_rank==0){                                                      // Proceso raiz 0

        arrayDesorden = new int[numElementos];
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
        
        // ******** Preguntar si puede especificarse que un proceso reciba de solo una etiqueta
        
        MPI_Bcast(&tamVecTmp, 1, MPI_INT, 0, MPI_COMM_WORLD);   /* Se encarga de enviarle a cada proceso el numero de elementos que le tocan a ese proceso.*/
        vecTemporal = new int[tamVecTmp];
        MPI_Scatter(arrayDesorden, tamVecTmp, MPI_INT, vecTemporal, tamVecTmp, MPI_INT, 0, MPI_COMM_WORLD);  /* El proceso raiz envia a todos los procesos incluido el mismo, los elementos
                                                                                                                a ordenar inicialmente, es decir, numElementos / numProcesos*/
        metodoOrde.mergeSort(vecTempral, 0, tamVecTmp-1);       /* Ordena la parte que le corresponde al proceso,
                                                                   esto es el proceso 0.*/
    }else{
        MPI_Bcast(&tamVecTmp, 1, MPI_INT, 0, MPI_COMM_WORLD);
        vecTemporal = new int[tamVecTmp];
        MPI_Scatter(arrayDesorden, tamVecTmp, vecTemporal, tamVecTmp, MPI_INT, 0, MPI_COMM_WORLD);
        metodoOrde.mergeSort(vecTemporal, 0, tamVecTmp-1);  /* Ordena lo que le corresponde */
    }
    
    // aqui debe hacerse el merge en cada proceso
    
    iteracion = 1;
    while(iteracion < numProcesos){
        if(my_rank%(2*iteracion) == 0){
            if(my_rank + iteracion < numProcesos){
                MPI_Recv(&cantActualProcs,1,MPI_INT,my_rank+iteracion,0,MPI_COMM_WORLD,&estado);
                vecMerge = new int[cantActualProcs];
                MPI_Recv(vecMerge,cantActualProcs,MPI_INT,my_rank+iteracion,0,MPI_COMM_WORLD,&estado);
                vecTemporal = metodoOrde.mezcla(vecTemporal,tamVecTmp,vecMerge,cantActualProcs);    /* Hace el merge */
                tamVecTmp = tamVecTmp+cantActualProcs;
            }
        }else{
            /* Ahora envio mi parte ordenada a otro proceso para juntar las partes ordenadas */
            int a_Quien = my_rank-iteracion;
            MPI_Send(&tamVecTmp,1,MPI_INT,a_Quien,0,MPI_COMM_WORLD);
            MPI_Send(vecTemporal,tamVecTmp,MPI_INT,a_Quien,0,MPI_COMM_WORLD);
            break;
        }
        iteracion = iteracion*2;
    }

    MPI_Finalize();
}

/*
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
0 2 4 6 8 10 12 14 16 18 20 22 24 26 28 30      --   la cantidad de ciclos es n, 2^n = numElementos
0 4 8 12 16 20 24 28
0 8 16 24                                       -- brinco 1, 3, 7
0 16
0

*/
















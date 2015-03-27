/**
  * Universidad de Costa Rica
  * Escuela de Ciencias de la Computacion e Informatica
  * Arquitectura de Computadoras
  * Tarea programada 1
  * @author Fabian Rodriguez B25695
  * @author Jose Pablo UreÃ±a B16692
  * I Semestre 2015
  * El programa implementa el algoritmo de merge sort con paralelismo y utilizando MPI para el paso de mensajes
  */

//#include "OrdenadorElementos.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <string>
#include "mpi.h"

using namespace std;


int* mezcla(int* vecA, int tamaA, int* vecB, int tamaB)
{
    int indexA = 0;
    int indexB = 0;
    int indexC = 0;
    int tamaVecC = tamaA+tamaB;         /* TamaÃ±o del nuevo vector */
    int* vecC = new int[tamaVecC];      /* Vector a retornar */

    while(indexA < tamaA && indexB < tamaB){
        if(vecA[indexA] <= vecB[indexB]){
            vecC[indexC] = vecA[indexA];
            ++indexC;
            ++indexA;
        }else{
            vecC[indexC] = vecB[indexB];
            ++indexB;
            ++indexC;
        }
    }

    int i = 0;
    /* Cuando hay un vector mas grande que otro */
    if(indexA >= tamaA){    // Vector A es mas grande
        for(i = indexC; i< tamaVecC; ++i){    /* VectorB ya esta ordenado entonces
                                                 solo tiene que ponerlo en vectorC */
            vecC[i] = vecB[indexB];
            ++indexB;
        }
    }else{
        if(indexB >= tamaB){    // Vector B es mas grande
            for(i = indexC; i< tamaVecC; ++i){    /* VectorA ya esta ordenado entonces
                                                     solo lo tiene que poner en vectorC */
                vecC[indexC] = vecA[indexA];
                ++indexA;
            }
        }
    }

    for(i = 0; i< tamaA; ++i){
        vecA[i] = vecC[i];
    }
    for(i = 0; i< tamaB; ++i){
        vecB[i] = vecC[tamaA+i];
    }
    return vecC;
}

void mergeSort(int* vec, int beg, int end)
{
    if(beg==end){
        return;
    }
    int mid = (beg+end)/2;
    mergeSort(vec,beg,mid);
    mergeSort(vec,mid+1,end);
    int i=beg,j=mid+1;
    int l=end-beg+1;
    int *temp = new int [l];
    for (int k=0;k<l;k++){
        if (j>end || (i<=mid && vec[i]<vec[j])){
            temp[k]=vec[i];
            i++;
        }
        else{
            temp[k]=vec[j];
            j++;
        }
    }
    for (int k=0,i=beg;k<l;k++,i++){
        vec[i]=temp[k];
    }
    delete temp;
}

int main(int argc, char **argv)
{

    int numProcesos;            /* Va a guardar la cantidad de procesos que el usuario quiere ejecutar. */
    int numElementos;           /* Va a guardar la cantidad de elementos que el usuario quiere que tenga el vector*/
    int tamVecTmp;              /* Cantidad de elementos que se destinan a cada proceso */
    int* vecTemporal;                                   /* Array de cada proceso */
    int* arrayDesorden;                                 /* Array que almacena todos los numeros en desorden*/
    int iteracion;                                      /* Para indicar en cual iteracion (o nivel del arbol) se encuentra */
    int* vecMerge;                                      /* En este array se van a acomodar los numeros de cada proceso mas los que recibe del otro. */
    int cantActualProcs;                                /* La cantidad de procesos que le tocan a cada subproceso. */
    MPI_Status estado;                                  /* Para saber cual es el estado del proceso. */
    int my_rank;					/* Indicador del proceso actual o identificador para el mismo*/

    MPI_Init(&argc, &argv);                                               /* Arranque del proceso MPI */

    MPI_Comm_size(MPI_COMM_WORLD, &numProcesos);                         /* El comunicador MPI_COMM_WORLD almacena el numero de procesos a ejecutar en la
                                                                            variable numProcesos */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);                             /* El comunicador MPI_COMM_WORLD almacena en la variable my_rank la etiqueta del
                                                                           proceso que realiza el llamado. */


    tamVecTmp = numElementos/numProcesos;               /* Cuantos elementos le tocan a cada proceso */

    if(my_rank==0){                                                      // Proceso raiz 0
        bool compatibles = false;
        arrayDesorden = new int[numElementos];
        string numeros = "";
        int valor = 0;
        stringstream ss;
        srand(time(NULL));

        cout<<"Ingrese la cantidad de números que desee ordenar: ";
        cin>>numElementos;
        if( (numElementos % numProcesos == 0) ){
            compatibles = true;
        }else{
            cout<<"Los datos ingresados no funcionan, la cantidad de elementos tiene que ser"<<endl;
            cout<<"divisible entre la cantidad de procesos. Vuelva a ingresar los datos"<<endl;
            cout<<"------------------------------------------------------------------------"<<endl;
        }

        ofstream numerosDesorden;                                           // Objeto Archivo
        numerosDesorden.open("ListaI.txt");                                 // Se crea el archivo que contiene los numeros en desorden
        numerosDesorden << "NÃºmeros en desorden: " << endl;

        for(int i = 0; i < numElementos; ++i){                                   // Se generan todos los numeros random
            valor =  rand()%2000 + 1 ;
            arrayDesorden[i] = valor;
            ss << valor;						// Se convierte el entero a string para guardarlo en el archivo
            numeros = ss.str();
            numerosDesorden << numeros+" ";                     // Se llena el archivo con valores random
            numeros = "";
            ss.str("");						/* Se limpia la variable stringstream*/
            if( (i % 10 == 0) && (i != 0)){
                numerosDesorden << "\n";
            }

        }
        
        MPI_Bcast(&tamVecTmp, 1, MPI_INT, 0, MPI_COMM_WORLD);   /* Se encarga de enviarle a cada proceso el numero de elementos que le tocan a ese proceso.*/
        vecTemporal = new int[tamVecTmp];
        MPI_Scatter(arrayDesorden, tamVecTmp, MPI_INT, vecTemporal, tamVecTmp, MPI_INT, 0, MPI_COMM_WORLD);  /* El proceso raiz envia a todos los procesos incluido el mismo, los elementos
                                                                                                                a ordenar inicialmente, es decir, numElementos / numProcesos*/
        mergeSort(vecTemporal, 0, tamVecTmp-1);       /* Ordena la parte que le corresponde al proceso,
                                                                   esto es el proceso 0.*/
    }else{
        MPI_Bcast(&tamVecTmp, 1, MPI_INT, 0, MPI_COMM_WORLD);
        vecTemporal = new int[tamVecTmp];
        MPI_Scatter(arrayDesorden, tamVecTmp, MPI_INT, vecTemporal, tamVecTmp, MPI_INT, 0, MPI_COMM_WORLD);
        mergeSort(vecTemporal, 0, tamVecTmp-1);  /* Ordena lo que le corresponde */
    }

    iteracion = 1;
    while(iteracion < numProcesos){
        if(my_rank%(2*iteracion) == 0){
            if(my_rank + iteracion < numProcesos){
                MPI_Recv(&cantActualProcs,1,MPI_INT,my_rank+iteracion,0,MPI_COMM_WORLD,&estado);
                vecMerge = new int[cantActualProcs];
                MPI_Recv(vecMerge,cantActualProcs,MPI_INT,my_rank+iteracion,0,MPI_COMM_WORLD,&estado);
                vecTemporal = mezcla(vecTemporal,tamVecTmp,vecMerge,cantActualProcs);    /* Hace el merge */
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

    string respuesta;				/* Variable encargada de almacenar la respuesta del usuario sobre una pregunta especifica*/

    ofstream numerosOrden;                                           // Objeto Archivo
    string numerosOrdenados;
    string numerosImprimir;
    stringstream ss2;						// Nuevo objeto stringstream para casting de valores enteros
    numerosOrden.open("ListaF.txt");                                 // Se crea el archivo que contiene los numeros en orden
    numerosOrden << "NÃºmeros en Orden: ";

    for(int m = 0; m < numElementos; ++m){                                   /* Se escribe en el archivo todos los numeros en orden */
        ss2 << vecTemporal[m];								// Se convierte el entero a string para guardarlo en el archivo
        numerosOrdenados = ss2.str();
        numerosOrden << numerosOrdenados+" ";                     // Se llena el archivo con los valores ordenados
        numerosImprimir = numerosImprimir+" "+numerosOrdenados;
        if( (m % 10 == 0) && (m != 0) ){
            numerosOrden << endl;
        }
        ss2.str("");
        numerosOrdenados = "";
    }

    cout << "¿Desea observar los números generados aleatoriamente ordenados?(Y/N): ";
    cin >> respuesta;

    if(respuesta == "Y"){
        cout << numerosImprimir << endl;
    }
    cout << "La lista de números ordenados se ha almacenado en el archivo: ListaF.txt" << endl;
    cout << endl;

    return 0;
}

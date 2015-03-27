#include "ordenador.h"

ordenador::ordenador()
{
}

int *ordenador::mezcla(int* vecA, int tamaA, int* vecB, int tamaB)
{
    int indexA = 0;
    int indexB = 0;
    int indexC = 0;
    int tamaVecC = tamaA+tamaB;         /* Tamaño del nuevo vector */
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

void ordenador::mergeSort(int* vec, int beg, int end)
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

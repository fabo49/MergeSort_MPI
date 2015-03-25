#include "ordenador.h"

ordenador::ordenador()
{
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

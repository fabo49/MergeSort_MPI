#ifndef ORDENADOR_H
#define ORDENADOR_H

class ordenador
{
private:
    int mergeSort_re();

public:
    ordenador();
    /**
     * @brief Implementacion del clasico algoritmo de merge sort
     * @author Fabian Rodriguez
     * @author Jose Pablo Ureña
     */
    void mergeSort(int* vec, int beg, int end);

    /**
     * @brief Se encarga de mezclar ordenadamente dos arrays.
     * @author Fabian Rodriguez
     * @author Jose Pablo Ureña
     * @return Array ordenado con la mezcla de los dos arrays que recibio.
     */
    int* mezcla(int*vecA, int tamaA, int*vecB, int tamaB);
};

#endif // ORDENADOR_H

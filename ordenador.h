/**
  * Universidad de Costa Rica
  * Escuela de Ciencias de la Computacion e Informatica
  * Arquitectura de Computadoras
  * Tarea programada 1
  * @author Fabian Rodriguez B25695
  * @author Jose Pablo Ureña B16692
  * I Semestre 2015
  * Metodos utilitarios usados por el main para ordenar usando merge sort
  * y para mezclar ordenadamente dos vectores
  */

#ifndef ORDENADOR_H
#define ORDENADOR_H

class ordenador
{
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

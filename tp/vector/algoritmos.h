#ifndef ALGORITMOS_H
#define ALGORITMOS_H

/// @file algoritmos.h
/// @brief Header con distintos algoritmos sobre el TDA Vector. Inspirados en C++

#include "vector.h"

/**
 *  @fn int transformarVector(Vector_t* vector, int (*Transformar)(void* dato, void* contexto), void* contexto)
 *  @brief Transforma los elementos dentro del vector segun la funcion transforma y el contexto otorgado, si lo fuese.
 *  @param vector Puntero al vector a transformar
 *  @param Transformar Puntero a la una funcion con firma int (*)(void*, void*). La misma realiza la transformacion
 *  sobre el primer elemento, y obtiene el contexto del segundo elemento.
 *  @param contexto Puntero a informacion adicional que se le tenga que pasar a la transformacion
 */
int transformarVector(Vector_t* vector, int (*Transformar)(void* dato, void* contexto), void* contexto);

/**
 *  @fn Vector_t* copiarVector(Vector_t* vector)
 *  @brief Realiza una copia del vector. Creando una nueva instancia en memoria de este
 *  @param vector Vector a copiar
 *  @warning La funcion realiza una nueva instancia dinamica del vector en memoria. Es responsabilidad
 *  del usuario conservar el puntero y liberar la memoria adecuadamente
 */
Vector_t* copiarVector(Vector_t* vector);

/**
 *  @fn Vector_t* mapearVector(Vector_t* vector, void* (*Mapear)(void* dato, void* tmp, void* contexto), size_t n, void* contexto)
 *  @brief Realiza un mapeo de un vector a otro, instanciando en memoria por la funcion, retorna un puntero a este nuevo vector.
 *  @param vector Vector a mapear
 *  @param Mapear Puntero a una funcion de firma void* (*)(void*, void*, void*). La misma realizara el mapeo sobre el primer elemento
 *  sobre el segundo elemento guarda los cambios, y del tercero agregara contexto.
 *  @param n Tamaño del elemento mapeado del nuevo vector instanciado.
 *  @param contexto Puntero a informacion adicional que se le tenga que pasar al mapeo
 *  @warning La funcion realiza una instancia dinamica de un vector en memoria. Es responsabilidad
 *  del usuario conservar el puntero y liberar la memoria adecuadamente
 */
Vector_t* mapearVector(Vector_t* vector, void* (*Mapear)(void* dato, void* tmp, void* contexto), size_t n, void* contexto);

/**
 *  @fn filtrarVector(Vector_t* vector, int (*Filtrar)(void* dato, void* contexto), void* contexto)
 *  @brief Realiza un filtrado de un vector a otro, instanciado en memoria por la funcio, retorna un puntero a este nuevo vector
 *  @param vector Vector a filtrar
 *  @param Filtrar Puntero a funcion de firma int (*)(void*, void*). Recibira en su primer elemento el elemento
 *  del vector a evaluar y en el segundo contexto adicional a agregar
 *  @param contexto Puntero a informacion adicional que se le tenga que pasar al filtrado
 *  @warning La funcion realiza una instancia dinamica de un vector en memoria. Es responsabilidad
 *  del usuario conservar el puntero y liberar la memoria adecuadamente
 */
Vector_t* filtrarVector(Vector_t* vector, int (*Filtrar)(void* dato, void* contexto), void* contexto);

/**
 *  @fn void* reducirVector(Vector_t* vector, void* (*Reducir)(void* dato, void* acumulado))
 *  @brief Realiza una reduccion de todos los elementos del vector a un unico elemento, que retorna por puntero
 *  @param vector Vector a reducir
 *  @param Reducir Puntero a funcion de firma void* (*)(void*, void*). Recibira por el primer parametro el elemento actual
 *  a evaluar del vector, en el segundo parametro el acumulador, retorna el acumulador.
 */
void* reducirVector(Vector_t* vector, void* (*Reducir)(void* dato, void* acumulado));

/**
 *  @fn Vector_t* reducirVectorPorClave(Vector_t* vector, void* (*ObtenerClave)(void* elem), int (*CompararClave)(void* lhs, void* rhs), void* (*Reducir)(void* elem, void* acumulado))
 *  @brief Realiza una reduccion por claves del vector, agrupando por claves el vector y reduciendo estos agrupamientos. Retorna una nueva instancia de un vector con los cambios hechos
 *  @param vector Vector a reducir por claves 
 *  @param ObtenerClave Funcion de firma void* (*)(void*), recibira el elemento actual del vector y retornara un puntero a la clave 
 *  @param CompararClave Funcion de firma int (*)(void*, void*). Recibira por el primer parametro la clave izquierda y en el segundo
 *  la clave derecha. Retorna cero si lhs es igual a rhs, mayor a cero si lhs es mayor a rhs o menor a cero si rhs es mayor a lhs.
 *  @param Reducir  Puntero a funcion de firma void* (*)(void*, void*). Recibira por el primer parametro el elemento actual
 *  a evaluar del vector, en el segundo parametro el acumulador, retorna el acumulador.
 *  @warning La funcion realiza una instancia dinamica de un vector en memoria. Es responsabilidad
 *  del usuario conservar el puntero y liberar la memoria adecuadamente
 */
Vector_t* reducirVectorPorClave(Vector_t* vector, void* (*ObtenerClave)(void* elem), int (*CompararClave)(void* lhs, void* rhs), void* (*Reducir)(void* elem, void* acumulado));

/**
 *  @fn Vector_t* unirVectores(Vector_t* lhs, Vector_t* rhs, void* (*Unir)(void* lhs, void* rhs, void* elem), size_t n)
 *  @brief Realiza la union entre dos vectores lhs y rhs de mismo tamaño obligatoriamente, y los guarda en una nueva instancia de vector que retorna.
 *  @param lhs Vector izquierdo
 *  @param rhs Vector derecho
 *  @param Unir Funcion de firma void* (*)(void*, void*, void*), recibe por el primer parametro un elemento del vector izquierdo
 *  por el segundo parametro uno del vector derecho, y por el tercer parametro el elemento donde se guardara la union
 *  @param n Tamaño del elemento unido del nuevo vector instanciado
 *  @warning La funcion realiza una instancia dinamica de un vector en memoria. Es responsabilidad
 *  del usuario conservar el puntero y liberar la memoria adecuadamente
 */
Vector_t* unirVectores(Vector_t* lhs, Vector_t* rhs, void* (*Unir)(void* lhs, void* rhs, void* elem), size_t n);

#endif
#ifndef CADENAS_H
#define CADENAS_H

///@file cadenas.h
///@brief Biblioteca para la manipulacion y examen de cadenas de caracteres

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <memory.h>
#include <ctype.h>
#include <string.h>

///@brief Manipulacion de cadenas

/** 
 *  @fn char* stringCopy(char* dest, char* src)
 *  @brief Realiza la copia de una cadena en otra
 *  @param dest Cadenas de destino de la operacion
 *  @param src Cadena de fuente de la operacion
 */
char* stringCopy(char* dest, char* src);

/**
*   @fn char* stringNCopy(char* dest, char* src, size_t n)
*   @brief Realiza la copia de hasta n caracteres, inclusive, de una cadena a otra
*   @param dest Cadenas de destino de la operacion
*   @param src Cadena de fuente de la operacion
*   @param n Cantidad de caracteres a copiar
 */
char* stringNCopy(char* dest, char* src, size_t n);

/**
 *  @fn char* stringDup(char* dest, char* src, size_t n)
 *  @brief Duplica la cadena str, usando memoria del heap
 *  @param str Cadena a duplicar
 *  @details Es la responsabilidad del usuario administrar la memoria alocada
 */
char* stringDup(char* str);

/** 
 *  @fn char* stringNDup(char* str, size_t n)
 *  @brief Duplica la cadena str hasta n caracteres, inclusive, usando memoria del heap
 *  @param str Cadena a duplicar
 *  @param n Cantidad de caracteres, inclusive, a duplicar
 *  @details Es la responsabilidad del usuario administrar la memoria alocada
 */
char* stringNDup(char* str, size_t n);

/**
 *  @fn char* stringCat(char* dest, char* src)
 *  @brief Concatena la cadena src al final de la cadena dest
 *  @param dest Cadena de destino de la concatenacion
 *  @param src Cadena a concatenar
 */
char* stringCat(char* dest, char* src);

/**
 *  @fn char* stringNCat(char* dest, char* src, size_t n)
 *  @brief Concatena la cadena src hasta n caracteres, inclusive, al final de la cadena dest
 *  @param dest Cadena de destino
 *  @param src Cadena fuente
 *  @param n Cantidad de caracteres a concatenar
 */ 
char* stringNCat(char* dest, char* src, size_t n);

/**
 * @fn char* stringToUpper
 * @brief Convierte todos los caracteres de la cadena a mayuscula
 * @param str Cadena a convertir
 */
char* stringToUpper(char* str);

/**
 *  @fn char* stringToLower
 *  @brief Convierte todos los caracteres de la cadena a minuscula
 *  @param str Cadena a convertir
 */
char* stringToLower(char* str);

/**
 *  @fn char* stringReverse(char* str)
 *  @brief Invierte la cadena
 *  @param str Cadena a invertir
 */
char* stringReverse(char* str);

/**
 *  @fn char* stringRemove(char* str)
 *  @brief Remueve todas las instancias del caracter especificado de la cadena
 *  @param str Cadena a evaluar
 *  @param ch Caracter a remover
 */
char* stringRemove(char* str, int ch);

/**
 *  @fn stringReplace(char* str, int ori, int new)
 *  @brief Reemplaza todas las instancias del caracter ori con el caracter new en la cadena str
 *  @param str Cadena a evaluar
 *  @param ori Caracter a reemplazar
 *  @param new Caracter reemplazante
 */
char* stringReplace(char* str, int ori, int new);

/**
 *  @fn char* stringTrim(char* dest, char* str, int i, int j)
 *  @brief Inserta la cadena str en la cadena dest, en el indice i de la cadena dest hasta el indice j
 *  @param dest Cadena a modificar
 *  @param str Cadena a insertar
 *  @param i Indice inicial de insercion
 *  @param j Indice final de insercion
 *  @warning La funcion no funciona. No usar.
 */
char* stringTrim(char* dest, char* str, int i, int j);

///@brief Examinacion de cadenas

/** 
 *  @fn int stringCmp(char* lhs, char* rhs)
 *  @brief Realiza la comparacion lexicografica entre lhs y rhs.
 *  @param lhs Cadena a la izquierda de la comparacion
 *  @param rhs Cadena a la derecha de la comparacion
 */
int stringCmp(char* lhs, char* rhs);

/**
 *  @fn int stringNCmp(char* lhs, char* rhs, size_t n)
 *  @brief Realiza la comparacion lexicografica entre lhs y rhs hasta los n caracteres.
 *  @param lhs Cadena a la izquierda de la comparacion
 *  @param rhs Cadena a la derecha de la comparacion
 *  @param n Cantidad de caracteres a evaluar
 */
int stringNCmp(char* lhs, char* rhs, size_t n);

/**
 *  @fn int stringCmpi(char* lhs, char* rhs)
 *  @brief Realiza la comparacion lexicografica entre lhs y rhs. Sin diferenciar mayusculas de minusculas
 *  @param lhs Cadena a la izquierda de la comparacion
 *  @param rhs Cadena a la derecha de la comparacion
 */
int stringCmpi(char* lhs, char* rhs);

/**
 *  @fn int stringNCmpi(char* lhs, char* rhs, size_t n)
 *  @brief Realiza la comparacion lexicografica entre lhs y rhs hasta los n caracteres. Sin diferenciar mayusculas de minusculas
 *  @param lhs Cadena a la izquierda de la comparacion
 *  @param rhs Cadena a la derecha de la comparacion
 *  @param n Cantidad de caracteres a evaluar
 */
int stringNCmpi(char* lhs, char* rhs, size_t n);

/**
 *  @fn char* stringChar(char* dest, int ch)
 *  @brief Busca la primer instancia del caracter ch dentro de la cadena dest y retorna un puntero a su posicion.
 *  @param dest Cadena a evaluar.
 *  @param ch Caracter a buscar.
 *  @returns Puntero a la primer instancia de ch en dest, o NULL sino se encuentra.
 */
char* stringChar(char* dest, int ch);

/**
 *  @fn char* stringRChar(char* dest, int ch)
 *  @brief Busca la ultima instancia del caracter ch dentro de la cadena dest y retorna un puntero a su posicion.
 *  @param dest Cadena a evaluar.
 *  @param ch Caracter a buscar.
 *  @returns Puntero a la ultima posicion del caracter ch en dest, o NULL sino se encuentra.
 */
char* stringRChar(char* dest, int ch);

/**
 *  @fn int stringCSpan(char* dest, char* src)
 *  @brief Retorna la cantidad de caracteres iniciales de dest que no forman parte de src
 *  @param dest Cadena a evaluar
 *  @param src Cadena con caracteres a buscar
 */
int stringCSpan(char* dest, char* src);

/**
 *  @fn int stringSpan(char* dest, char* src)
 *  @brief Retorna la cantidad de caracteres iniciales de dest que forman parte de src
 *  @param dest Cadena a evaluar
 *  @param src Cadena con caracteres a buscar
 */
int stringSpan(char* dest, char* src);

/**
 *  @fn char* stringPBreak(char* dest, char* src)
 *  @brief Retorna un puntero a la primer instancia de un caracter de src en dest. O NULL sino lo encuentra
 *  @param dest Cadena a evaluar
 *  @param src Cadena con caracteres a buscar
 */
char* stringPBreak(char* dest, char* src);

/**
 *  @fn char* stringSubstring(char* dest, char* src)
 *  @brief Retorna un puntero a la primer instancia de la subcadena src dentro de dest. O NULL sino la encuenta
 *  @param dest Cadena a evaluar
 *  @param src Subcadena a buscar
 */
char* stringSubstring(char* dest, char* src);

/**
 *  @fn int stringLenght(const char* str)
 *  @brief Retorna el largo de la cadena str
 *  @param str Cadena a evaluar
 */
int stringLenght(const char* str);

///@brief Utilidades varias

/**
 *  @fn int memzero(void* ptr, size_t n)
 *  @brief Convierte los n bytes desde apuntados por ptr a 0.
 *  @param ptr Puntero la informacion.
 *  @param n Cantidad de bytes a convertir en 0.
 */
int memzero(void* ptr, size_t n);

/**
 *  @fn char* scanString(char* str, size_t n)
 *  @brief Escanea de stdin la hasta n caracteres, los cuales guarda en str, luego sanitiza la cadena.
 *  @param str Cadena donde se guardara la informacion escaneada.
 *  @param n Cantidad de n caracteres a leer de stdin.
 */
char* scanString(char* str, size_t n);

/**
 *  @fn void flushStdin(void)
 *  @brief Realiza un flush seguro del stdin, consumiendo todos los caracteres que se encuentren en el stream.
 */
void flushStdin(void);

#endif
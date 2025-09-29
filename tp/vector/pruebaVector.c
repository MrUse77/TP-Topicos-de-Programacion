#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

int compararEmpleado(void* emplA, void* emplB);
void mostrarEmpleado(void* empleado);

typedef struct{
    char nomApe[50];
    char departamento[50];
    int legajo;
    double sueldo;
}Empleado;

int main()
{
    Vector_t vecA, vecB, vecC;

    vectorCrear(&vecA, sizeof(Empleado));

    Empleado emp1 = {"Juan Perez", "Sistemas", 1234, 55000};
    Empleado emp2 = {"Ana Gomez", "Sistemas", 5678, 60000};
    Empleado emp3 = {"Luis Martinez", "Ventas", 9101, 45000};
    Empleado emp4 = {"Maria Lopez", "Ventas", 1121, 47000};
    Empleado emp5 = {"Carlos Sanchez", "RRHH", 3141, 52000};
    Empleado emp6 = {"Laura Fernandez", "RRHH", 5161, 53000};
    Empleado emp7 = {"Pedro Alvarez", "Sistemas", 7181, 58000};

    vectorInsertarOrd(&vecA, &emp1, compararEmpleado);
    vectorInsertarOrd(&vecA, &emp2, compararEmpleado);
    vectorInsertarOrd(&vecA, &emp3, compararEmpleado);
    vectorInsertarOrd(&vecA, &emp4, compararEmpleado);
    vectorInsertarOrd(&vecA, &emp5, compararEmpleado);
    vectorInsertarOrd(&vecA, &emp6, compararEmpleado);
    vectorInsertarOrd(&vecA, &emp7, compararEmpleado);

    mostrarVector(&vecA, mostrarEmpleado);

    vectorEliminarPos(&vecA, 0);

    puts("");

    mostrarVector(&vecA, mostrarEmpleado);

    vectorDestruir(&vecA);

    return 0;
}

void mostrarEmpleado(void* empleado)
{
    Empleado* tmp = (Empleado*) empleado;

    printf("%s\t%s\t%05d\t $%0.2f\n", tmp->nomApe, tmp->departamento, tmp->legajo, tmp->sueldo);
}

int compararEmpleado(void* emplA, void* emplB)
{
    Empleado* tmpA = (Empleado*) emplA;
    Empleado* tmpB = (Empleado*) emplB;

    return (tmpA->legajo - tmpB->legajo);
}
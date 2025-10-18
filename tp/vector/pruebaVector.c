#include "vector.h"
#include "iterador.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ELEMS 10

int compararEmpleado(void* emplA, void* emplB);
void mostrarEmpleado(void* empleado);

int compararInt(void* intA, void* intB);
void mostrarInt(void* n);

typedef struct{
    char nomApe[50];
    char departamento[50];
    int legajo;
    double sueldo;
}Empleado;

Empleado generarEmpleadoAleatorio(void);

int main()
{
    Vector_t vecA;

    srand(1);

    Empleado tmp;
    Empleado* ptr;

    vectorCrear(&vecA, sizeof(Empleado));

    for(int i = 0; i < ELEMS; i++){
        tmp = generarEmpleadoAleatorio();
        vectorEmpujar(&vecA, &tmp);
    }
    
    int seg_ini = time(NULL);

    vectorOrdenar(&vecA, compararEmpleado);

    Iterador_t iterA;

    iteradorCrear(&iterA, &vecA);

    while((ptr = iteradorSiguiente(&iterA))){
        mostrarEmpleado(ptr);
    }

    iteradorMoverCursor(&iterA, -2, ITER_FIN);

    while((ptr = iteradorSiguiente(&iterA))){
        mostrarEmpleado(ptr);
    }

    iteradorDestruir(&iterA);

    int seg_fin = time(NULL);

    printf("%d\n", seg_fin - seg_ini);

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

int compararInt(void* intA, void* intB)
{
    return (*(long*)intA - *(long*)intB);
}

void mostrarInt(void* n)
{
    printf("[%02ld]\n", *(long*)n);
}

Empleado generarEmpleadoAleatorio() {
    // Datos de prueba
    const char* nombres[] = {"Ana", "Juan", "Pedro", "Lucia", "Sofia", "Carlos", "Maria", "Luis"};
    const char* apellidos[] = {"Gomez", "Perez", "Fernandez", "Lopez", "Diaz", "Martinez", "Ruiz", "Sanchez"};
    const char* departamentos[] = {"Ventas", "Recursos Humanos", "IT", "Marketing", "Finanzas", "Logistica"};

    int cantNombres = sizeof(nombres) / sizeof(nombres[0]);
    int cantApellidos = sizeof(apellidos) / sizeof(apellidos[0]);
    int cantDeptos = sizeof(departamentos) / sizeof(departamentos[0]);

    Empleado e;

    // Generar nombre y apellido combinados
    snprintf(e.nomApe, sizeof(e.nomApe), "%s %s",
             nombres[rand() % cantNombres], apellidos[rand() % cantApellidos]);

    // Departamento
    strcpy(e.departamento, departamentos[rand() % cantDeptos]);

    // Legajo (ejemplo entre 1000 y 9999)
    e.legajo = 1000 + rand() % 9000;

    // Sueldo (ejemplo entre 50,000 y 200,000)
    e.sueldo = 50000 + (rand() % 150001);

    return e;
}
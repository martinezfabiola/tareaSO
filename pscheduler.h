/*****************************************************************************
* Universidad Simon Bolivar
* Departamento de Computacion y Tecnologia de la Informacion
* CI3825: Laboratorio de Sistemas Operativos 
*
* Tarea 1
*
* Objetivo: Adquisicion de destrezas de programacion en el lenguaje C
*
* Autores: 
*   - David Cabeza 13-10191
*   - Fabiola Martinez 13-10838
* Equipo: N
*
* Prof. Angela Di Serio
*
* Febrero, 2017.
*****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*

Este archivo contiene la definción de las estructuras y los prototipos de las
funciones.

*/

typedef char* Estado;

typedef struct proceso
{
	long PID;
	float Time;
	char Comando[100];
	Estado Estado;
} Proceso;

typedef struct nodo
{
	Proceso *proceso;
	void *next;
} NODO;

typedef struct cola
{
	NODO *primero, *ultimo;
	int size;
	int nro;
} COLA;

typedef struct EstrucSched
{
	COLA *q0, *q1, *q2, *q3, *q4, *q5;
	NODO *enEjecucion;

} EstrucSched;

// Prototipos de funciones principales 

void InsertarProceso(EstrucSched *s, Proceso *p, short prioridad);

void ElimProceso(EstrucSched *s, long pid, short prio);

void ElimProcesoE(EstrucSched *s);

Proceso *ProxProceso(EstrucSched *s);

void CambiarEstado(EstrucSched *s, Proceso* p, Estado newestado);

EstrucSched *Construye(char *filename);

void Imprime(EstrucSched *s);

// Prototipos de funciones secundarias

void insertarProc(COLA *q, Proceso *p);

Proceso *ProxProc(EstrucSched *s, COLA *q);

void Imprimir(COLA *q);

void init_proceso(Proceso *p, long procesoid, float time, char estado, char* comando);

void init_nodo(NODO *n, Proceso *p);

void init_cola(COLA *q);

void init_EstrucSched(EstrucSched *E);
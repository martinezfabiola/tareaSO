/***************************************************************************************************
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
***************************************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*

Este archivo contiene la definción de las estructuras y los prototipos de las
funciones.

*/

typedef char Estado;

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
	struct nodo *next;
	struct nodo *prev;
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
	COLA *enEjecucion;

} EstrucSched;

// Prototipos de funciones principales 

/*	
	- InsertarProceso: permite insertar un proceso en la cola de prioridad que 
	corresponda.
	- Parametros de entrada: s apuntador a una estructura de cola tipo EstrucShed, 
	p apuntador al proceso, prioridad entero que indica la prioridad de la cola 
	donde se insertara el proceso.
*/
void InsertarProceso(EstrucSched *s, Proceso *p, short prioridad);

/*
- ElimProceso: se encarga de eliminar un proceso de la estructura de colas.
- Parametros de entrada: apuntador a la estructura de cola tipo EstrucShed, i-
dentificador del proceso a eliminar y prioridad de la cola donde se quiere eli-
minar el proceso.
- Parametros de salida:
*/
void ElimProceso(EstrucSched *s, long pid, short prio);

/*
- ElimProcesoE: se encarga de eliminar el próximo proceso a ejecutar, el único 
que tiene en su estado el valor de E (EnEjecucion).
- Parametros de entrada: apuntador a la estructura de cola tipo EstrucShed en la 
que se quiere eliminar el proceso.
*/
void ElimProcesoE(EstrucSched *s);

/*
- ProxProc: permite obtener el siguiente proceso a planificar.
- Parametros de entrada: apuntador a la estructura de cola tipo EstrucShed y a-
puntador a la cola en la que se buscara el proximo proceso.
- Parametros de salida: objeto tipo proceso que representa el proximo proceso a 
planificar.
*/
Proceso *ProxProceso(EstrucSched *s);

/*
- CambiarEstado: permite cambiar el valor del estado de un proceso, suponiendo 
que los valores introducidos por el usuario son Listo o EnEjecucion.
- Parametros de entrada: apuntador a la estructura de colas tipo EstrucShed, a-
puntador al proceso al cual se le quiere cambiar estado.
- Parametros de salida:
*/
void CambiarEstado(EstrucSched *s, Proceso* p, Estado newestado);

/*  
	- Construye: inicializa el planificador de procesos desde un archivo de texto.
	- Parametros de entrada: nombre del archivo de texto que contiene la infor-
	mación del planificador.
	- Parametros de salida: estructura tipo EstrucSched que representa el plani-
	ficador de procesos
*/
EstrucSched *Construye(char *filename);

/*
- Imprime: se encarga de mostrar por salida estandar el estado correspondiente a 
cada cola.
- Parametros de entrada: apuntador a la estructura de cola tipo EstrucShed.
- Parametros de salida:
*/
void Imprime(EstrucSched *s);

// Prototipos de funciones secundarias

/*	
	- insertarProc: procedimiento auxiliar para insertar un proceso en un cola.
	- Parametros de entrada: q apuntador a la cola en la cual se insertará el 
	  proceso y p apuntador al proceso a insertar.
*/
void insertarProc(COLA *q, Proceso *p);

/*
- ProxProc: permite obtener el siguiente proceso a planificar.
- Parametros de entrada: apuntador a la estructura de cola tipo EstrucShed y a-
puntador a la cola en la que se buscara el proximo proceso.
- Parametros de salida: objeto tipo proceso que representa el proximo proceso a 
planificar.
*/
Proceso *ProxProc(EstrucSched *s, COLA *q);

/*
- Imprimir: perimite mostrar la informacion correspondiente a cada cola sobre 
PID, Estado, Tiempo y Comando.
- Parametros de entrada: apuntador a la cola a la cual se quiere mostrar su estado.
- Parametros de salida:
*/
void Imprimir(COLA *q);

/*
- init_proceso: procedimiento que inicializa un proceso.
- Parametros de entrada: apuntador al proceso a inicializar, identificador del 
  proceso, tiempo ficticio, estado del proceso y comando.
*/
void init_proceso(Proceso *p, long procesoid, float time, char estado, char* comando);

/*
- init_nodo: procedimiento que inicializa un nodo.
- Parametros de entrada: apuntador al nodo a inicializar y apuntador al proceso 
  de dicho nodo.
*/
void init_nodo(NODO *n, Proceso *p);

/*
- init_cola: procedimiento que inicializa una cola de nodos.
- Parametros de entrada: apuntador a la cola a inicializar y entero que indica 
  la prioridad de la cola.
*/
void init_cola(COLA *q, int numero);

/*
- init_EstrucSched: procedimiento que inicializa una estructura de colas.
- Parametros de entrada: apuntador a la estructura de cola a inicializar.
*/
void init_EstrucSched(EstrucSched *E);

/*
- Salida: se encarga se escribir en el archivo el estado actual de cada cola.
- Parametros de entrada: apuntador a la estructura de colas y el nombre del ar-
chivo de salida donde se guardaran los datos.
- Parametros de salida: 
*/
int Salida(EstrucSched *E, char *filename);

/*
- writeSalida: se encarga de escribir en el archivo de salida los de una cola.
- Parametros de entrada: apuntador a la cola 
- Parametros de salida: 
*/
void writeSalida(COLA *q, FILE *f);

/*
- ProcEnEJec: permite obtener el apuntador al proceso que se encuentra en ejecu-
ción.
- Parametros de entrada: apuntador a la estructura de colas.
- Parametros de salida: objeto tipo Nodo que contiene el proceso en ejecución.
*/
NODO *ProcEnEjec(EstrucSched *s);
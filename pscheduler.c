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

/*
	Este es el programa principal.
*/

#include "pscheduler.h"

/*
- init_EstrucShec: permite inicializar una estructura de colas.
- Parametros de entrada: apuntador a la estructura de cola a inicializar.
- Parametros de salida:
*/
void init_EstrucSched(EstrucSched *E){
	E->q0 = (COLA *) malloc(sizeof(COLA));
	E->q1 = (COLA *) malloc(sizeof(COLA));
	E->q2 = (COLA *) malloc(sizeof(COLA));
	E->q3 = (COLA *) malloc(sizeof(COLA));
	E->q4 = (COLA *) malloc(sizeof(COLA));
	E->q5 = (COLA *) malloc(sizeof(COLA));

	E->enEjecucion = NULL;

	init_cola(E->q0, 0);
	init_cola(E->q1, 1);
	init_cola(E->q2, 2);
	init_cola(E->q3, 3);
	init_cola(E->q4, 4);
	init_cola(E->q5, 5);
}

/*
- init_cola: permite inicializar una cola.
- Parametros de entrada: apuntador a la cola a inicializar y entero que indica la prioridad de la 
cola.
- Parametros de salida:
*/
void init_cola(COLA *q, int numero){
	q->primero = NULL;
	q->ultimo = NULL;
	q->size = 0;
	q->nro = numero;
}

/*
- init_nodo: permite inicializar un nodo.
- Parametros de entrada: apuntador al nodo a inicializar y apuntador al proceso de dicho nodo.
- Parametros de salida:
*/
void init_nodo(NODO *n, Proceso *p){
	n->proceso = p;
	n->next = NULL;
	n->prev = NULL;
}

/*
- init_proceso: permite inicializar un proceso.
- Parametros de entrada: apuntador al proceso a inicializar, identificador del proceso, tiempo fic-
ticio, estado del proceso y comando.
- Parametros de salida:
*/
void init_proceso(Proceso *p, long procesoid, float time, char estado, char* comando){
	p->PID = procesoid;
	p->Time = time;
	p->Estado = estado;
	strcpy(p->Comando, comando);
}

/*  
	- Construye: inicializa cada cola a partir de un archivo de texto.
	- Parametros de entrada: nombre del archivo de texto que contiene la información de estado ini-
	cial correspondiente a cada cola.
	- Parametros de salida: estructura tipo EstrucSched que contiene cada cola con los datos leidos,
	una estructura de colas.
*/
EstrucSched* Construye(char *filename){

	// Inicializamos apuntador a estructura tipo EstrucShed que contiene la colas
	EstrucSched *E = (EstrucSched *) malloc(sizeof(EstrucSched));
	init_EstrucSched(E);

	// Inicializamos los datos de procesos a leer
	long PID;
	char Estado;
	short Prioridad;
	float Time;
	char Comando[100];

	// Leemos archivo 
	FILE *fp = fopen(filename, "r");

	while(fscanf(fp, "%li %c %hi %f %s", &PID, &Estado, &Prioridad, &Time, Comando) != EOF){

		// Insertamos proceso leido del archivo de texto en la cola correspondiente
		Proceso *p = (Proceso *) malloc(sizeof(Proceso));
		init_proceso(p, PID, Time, Estado, Comando);
		InsertarProceso(E, p, Prioridad);
	}

	return E;
	
}

/*	
	- InsertarProceso: permite insertar un proceso en la cola de prioridad que corresponda.
	- Parametros de entrada: apuntador a una estructura de cola tipo EstrucShed, apuntador al proce-
	so y entero que indica la prioridad de la cola a insertar proceso.
	- Parametros de salida:
*/
void InsertarProceso(EstrucSched* s, Proceso *p, short prioridad){
	
	switch(prioridad){
		case 0:
			insertarProc(s->q0, p);
			break;
		case 1:
			insertarProc(s->q1, p);
			break;
		case 2:
			insertarProc(s->q2, p);
			break;
		case 3:
			insertarProc(s->q3, p);
			break;
		case 4:
			insertarProc(s->q4, p);
			break;
		case 5:
			insertarProc(s->q5, p);
			break;
	}
}

/*	
	- insertarProc: permite insertar un proceso en un cola.
	- Parametros de entrada: apuntador a la cola a la cual se insertará el proceso y apuntador al 
	proceso a insertar.
	- Parametros de salida:
*/
void insertarProc(COLA *q, Proceso *p){
	
	NODO *n = (NODO *) malloc(sizeof(NODO));
	init_nodo(n, p);
	
	if(!q->primero){
		q->primero = n;
		q->ultimo = n;
	}

	else{
		q->ultimo->next = n;
		n->prev = q->ultimo;
		q->ultimo = n;
	}

	q->size++;
}

/*
- ElimProcesoE: se encarga de eliminar el próximo proceso a ejecutar, el único que tiene en su esta-
el valor de E (EnEjecucion).
- Parametros de entrada: apuntador a la estructura de cola tipo EstrucShed en la que se quiere eli-
minar el proceso.
- Parametros de salida:
*/
void ElimProcesoE(EstrucSched *s){
	
	if(s->enEjecucion){

		NODO *nodo = s->enEjecucion->primero;
		NODO *proximo = nodo->next;
		
		if (proximo){
			while(proximo->next && nodo->proceso->Estado != 'E'){
				nodo = proximo;
				proximo = nodo->next;
			}
			nodo->next = NULL;
			s->enEjecucion->ultimo = nodo;
			free(proximo);
		}
		else{
			s->enEjecucion->primero = NULL;
			s->enEjecucion->ultimo = NULL;
			free(nodo);
		}

		s->enEjecucion->size--;
		s->enEjecucion = NULL;
	}
	else{
		printf("No hay ningun proceso en ejecucion.\n");
	}
}


/*
- ProxProceso:
- Parametros de entrada:
- Parametros de salida:
*/
Proceso *ProxProceso(EstrucSched *s){

	if(s->enEjecucion){
		printf("Ya existe un proceso en ejecucion. Intente eliminarlo primero.\n");
		return NULL;
	}
	
	Proceso *p;

	p = ProxProc(s, s->q0);

	if(p) return p;

	p = ProxProc(s, s->q1);

	if(p) return p;

	p = ProxProc(s, s->q2);

	if(p) return p;

	p = ProxProc(s, s->q3);

	if(p) return p;

	p = ProxProc(s, s->q4);

	if(p) return p;

	p = ProxProc(s, s->q5);

	if(p) return p;

	return NULL;
}

/*
- ProxProc: permite obtener el siguiente proceso a planificar.
- Parametros de entrada: apuntador a la estructura de cola tipo EstrucShed y apuntador a la cola en 
la que se buscara el proximo proceso.
- Parametros de salida: objeto tipo proceso que representa el proximo proceso a planificar.
*/
Proceso *ProxProc(EstrucSched *s, COLA *q){

	if(q->primero != NULL){
		q->primero->proceso->Estado = 'E';
		q->ultimo->next = q->primero;
		q->ultimo = q->primero;
		q->primero = q->primero->next;
		q->ultimo->next = NULL;
		s->enEjecucion = q;
		return q->ultimo->proceso;
	}
	return NULL;
}

/*
- ElimProceso: se encarga de eliminar un proceso de la estructura de colas.
- Parametros de entrada: apuntador a la estructura de cola tipo EstrucShed, identificador del proce-
so a eliminar y prioridad de la cola donde se quiere eliminar el proceso.
- Parametros de salida:
*/
void ElimProceso(EstrucSched *s, long pid, short prio){
	COLA *cola = NULL;

	if (prio == 0) cola = s->q0;
	if (prio == 1) cola = s->q1;
	if (prio == 2) cola = s->q2;
	if (prio == 3) cola = s->q3;
	if (prio == 4) cola = s->q4;
	if (prio == 5) cola = s->q5;

	NODO* tmp = cola->primero;
	
	while(tmp != NULL) {
		if (tmp->proceso->PID == pid) {
			if (cola->size == 1){
				cola->primero = NULL;
				cola->ultimo = NULL;
				free(tmp->proceso);
				free(tmp);
			}

			else{
				tmp->prev = tmp->next;
				tmp->next = NULL;
				free(tmp->next->proceso);
				free(tmp->next);
			}
			break;
		}
		tmp = tmp->next;
	}
}

/*
- CambiarEstado: permite cambiar el valor del estado de un proceso, suponiendo que los valores in-
troducidos por el usuario son Listo o EnEjecucion.
- Parametros de entrada: apuntador a la estructura de colas tipo EstrucShed, apuntador al proceso al
cual se le quiere cambiar estado.
- Parametros de salida:
*/
void CambiarEstado(EstrucSched *s, Proceso* p, Estado newestado){
	p->Estado = newestado;
}

/*
- Imprime: se encarga de mostrar por salida estandar el estado correspondiente a cada cola.
- Parametros de entrada: apuntador a la estructura de cola tipo EstrucShed.
- Parametros de salida:
*/
void Imprime(EstrucSched *s){
	
	if(s->q0->size != 0) Imprimir(s->q0);

	if(s->q1->size != 0) Imprimir(s->q1);		

	if(s->q2->size != 0) Imprimir(s->q2);
		
	if(s->q3->size != 0) Imprimir(s->q3);

	if(s->q4->size != 0) Imprimir(s->q4);

	if(s->q5->size != 0) Imprimir(s->q5);
}

/*
- Imprimir: perimite mostrar la informacion correspondiente a cada cola sobre PID, Estado, Tiempo y 
Comando.
- Parametros de entrada: apuntador a la cola a la cual se quiere mostrar su estado.
- Parametros de salida:
*/
void Imprimir(COLA *q){

	NODO *first = q->primero;
	printf("Cola %d\n", q->nro);
	printf("PID\tEstado\tTiempo\tComando\n");
	printf("%li\t%c\t%.2f\t%s\n", first->proceso->PID, first->proceso->Estado, first->proceso->Time, first->proceso->Comando);
	
	NODO *proximo = first->next;

	while(proximo != NULL){
		printf("%li\t%c\t%.2f\t%s\n", proximo->proceso->PID, proximo->proceso->Estado, proximo->proceso->Time, proximo->proceso->Comando);
		proximo = proximo->next;
	}
}

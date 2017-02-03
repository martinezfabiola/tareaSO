/*******************************************************************************
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
********************************************************************************/
#include "pscheduler.h"

/*
- init_EstrucSched: procedimiento que inicializa una estructura de colas.
- Parametros de entrada: apuntador a la estructura de cola a inicializar.
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
- init_cola: procedimiento que inicializa una cola de nodos.
- Parametros de entrada: apuntador a la cola a inicializar y entero que indica 
  la prioridad de la cola.
*/
void init_cola(COLA *q, int numero){
	q->primero = NULL;
	q->ultimo = NULL;
	q->size = 0;
	q->nro = numero;
}

/*
- init_nodo: procedimiento que inicializa un nodo.
- Parametros de entrada: apuntador al nodo a inicializar y apuntador al proceso 
  de dicho nodo.
*/
void init_nodo(NODO *n, Proceso *p){
	n->proceso = p;
	n->next = NULL;
	n->prev = NULL;
}

/*
- init_proceso: procedimiento que inicializa un proceso.
- Parametros de entrada: apuntador al proceso a inicializar, identificador del 
  proceso, tiempo ficticio, estado del proceso y comando.
*/
void init_proceso(Proceso *p, long procesoid, float time, char estado, char* comando){
	p->PID = procesoid;
	p->Time = time;
	p->Estado = estado;
	strcpy(p->Comando, comando);
}

/*  
	- Construye: inicializa el planificador de procesos desde un archivo de texto.
	- Parametros de entrada: nombre del archivo de texto que contiene la infor-
	mación del planificador.
	- Parametros de salida: estructura tipo EstrucSched que representa el plani-
	ficador de procesos
*/
EstrucSched* Construye(char *filename){

	// Inicializamos apuntador a estructura tipo EstrucShed que contiene la colas
	EstrucSched *E = (EstrucSched *) malloc(sizeof(EstrucSched));
	init_EstrucSched(E);

	long PID;
	char Estado;
	short Prioridad;
	float Time;
	char Comando[100];

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
	- InsertarProceso: permite insertar un proceso en la cola de prioridad que 
	corresponda.
	- Parametros de entrada: s apuntador a una estructura de cola tipo EstrucShed, 
	p apuntador al proceso, prioridad entero que indica la prioridad de la cola 
	donde se insertara el proceso.
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
		default:
			break;
	}
}

/*	
	- insertarProc: procedimiento auxiliar para insertar un proceso en un cola.
	- Parametros de entrada: q apuntador a la cola en la cual se insertará el 
	  proceso y p apuntador al proceso a insertar.
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
- ElimProcesoE: se encarga de eliminar el próximo proceso a ejecutar, el único 
que tiene en su estado el valor de E (EnEjecucion).
- Parametros de entrada: apuntador a la estructura de cola tipo EstrucShed en la 
que se quiere eliminar el proceso.
*/
void ElimProcesoE(EstrucSched *s){
	
	if(s->enEjecucion){
		NODO *EnEjecucion = ProcEnEjec(s);
		if (s->enEjecucion->size == 1){
			s->enEjecucion->primero = NULL;
			s->enEjecucion->ultimo = NULL;
		}
		else{
			if(EnEjecucion->prev){
				EnEjecucion->prev->next = EnEjecucion->next;
			}

			if(EnEjecucion->next){
				EnEjecucion->next->prev = EnEjecucion->prev;
			}
		}
		free(EnEjecucion->proceso);
		free(EnEjecucion);
		s->enEjecucion->size--;
		s->enEjecucion = NULL;
	}
}


/*
- ProxProceso: se encarga de buscar en las colas el proximo proceso que este lis-
para ejecutar de cambiando su estado de Listo a EnEjecucion
- Parametros de entrada: apuntador a la estructura de colas.
- Parametros de salida: apuntador al proximo proceso a ejecutar.
*/
Proceso *ProxProceso(EstrucSched *s){

	if(s->enEjecucion) return NULL;
	
	if (s->q0->size > 0) return ProxProc(s, s->q0);

	else if (s->q1->size > 0) return ProxProc(s, s->q1);

	else if (s->q2->size > 0) return ProxProc(s, s->q2);

	else if (s->q3->size > 0) return ProxProc(s, s->q3);

	else if (s->q4->size > 0) return ProxProc(s, s->q4);

	else if (s->q5->size > 0) return ProxProc(s, s->q5);

	return NULL;
}

/*
- ProxProc: permite obtener el siguiente proceso a planificar.
- Parametros de entrada: apuntador a la estructura de cola tipo EstrucShed y a-
puntador a la cola en la que se buscara el proximo proceso.
- Parametros de salida: objeto tipo proceso que representa el proximo proceso a 
planificar.
*/
Proceso *ProxProc(EstrucSched *s, COLA *q){

	NODO *tmp = q->primero;
	q->primero = tmp->next;
	q->primero->prev = NULL;

	tmp->prev = q->ultimo;
	q->ultimo->next = tmp;
	q->ultimo = tmp;
	q->ultimo->next = NULL;

	q->ultimo->proceso->Estado = 'E';
	s->enEjecucion = q;
	return q->ultimo->proceso;
}

/*
- ElimProceso: se encarga de eliminar un proceso de la estructura de colas.
- Parametros de entrada: apuntador a la estructura de cola tipo EstrucShed, i-
dentificador del proceso a eliminar y prioridad de la cola donde se quiere eli-
minar el proceso.
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
			}

			else{
				if(tmp->prev){
					tmp->prev->next = tmp->next;
				}
				if(tmp->next){
					tmp->next->prev = tmp->prev;
				}
			}
			break;
		}
		tmp = tmp->next;
	}
	free(tmp->proceso);
	free(tmp);
	cola->size--;
}

/*
- CambiarEstado: permite cambiar el valor del estado de un proceso, suponiendo 
que los valores introducidos por el usuario son Listo o EnEjecucion.
- Parametros de entrada: apuntador a la estructura de colas tipo EstrucShed, a-
puntador al proceso al cual se le quiere cambiar estado.
- Parametros de salida:
*/
void CambiarEstado(EstrucSched *s, Proceso *p, Estado newestado){
	if (p == ProcEnEjec(s)->proceso){
		s->enEjecucion = NULL;
	}
	p->Estado = newestado;
}

/*
- Imprime: se encarga de mostrar por salida estandar el estado correspondiente a 
cada cola.
- Parametros de entrada: apuntador a la estructura de cola tipo EstrucShed.
- Parametros de salida:
*/
void Imprime(EstrucSched *s){
	
	if(s->q0->size != 0) Imprimir(s->q0);
	else printf("Cola 0\nEsta cola se encuentra vacia\n\n");

	if(s->q1->size != 0) Imprimir(s->q1);
	else printf("Cola 1\nEsta cola se encuentra vacia\n\n");

	if(s->q2->size != 0) Imprimir(s->q2);
	else printf("Cola 2\nEsta cola se encuentra vacia\n\n");
		
	if(s->q3->size != 0) Imprimir(s->q3);
	else printf("Cola 3\nEsta cola se encuentra vacia\n\n");

	if(s->q4->size != 0) Imprimir(s->q4);
	else printf("Cola 4\nEsta cola se encuentra vacia\n\n");

	if(s->q5->size != 0) Imprimir(s->q5);
	else printf("Cola 5\nEsta cola se encuentra vacia\n\n");	
}

/*
- Imprimir: perimite mostrar la informacion correspondiente a cada cola sobre 
PID, Estado, Tiempo y Comando.
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

	printf("\n");
}

/*
- Salida: se encarga se escribir en el archivo el estado actual de cada cola.
- Parametros de entrada: apuntador a la estructura de colas y el nombre del ar-
chivo de salida donde se guardaran los datos.
- Parametros de salida: 
*/
int Salida(EstrucSched *s, char *filename){

	FILE *f = fopen(filename, "w");

	if (!f){
		printf("No se pudo abrir el archivo, invoque la funcion nuevamente");
		return -1;
	}

	if(s->q0->size != 0) writeSalida(s->q0, f);
	else fprintf(f, "Cola 0\nEsta cola se encuentra vacia\n\n");

	if(s->q1->size != 0) writeSalida(s->q1, f);
	else fprintf(f, "Cola 1\nEsta cola se encuentra vacia\n\n");

	if(s->q2->size != 0) writeSalida(s->q2, f);
	else fprintf(f, "Cola 2\nEsta cola se encuentra vacia\n\n");

	if(s->q3->size != 0) writeSalida(s->q3, f);
	else fprintf(f, "Cola 3\nEsta cola se encuentra vacia\n\n");

	if(s->q4->size != 0) writeSalida(s->q4, f);
	else fprintf(f, "Cola 4\nEsta cola se encuentra vacia\n\n");

	if(s->q5->size != 0) writeSalida(s->q5, f);
	else fprintf(f, "Cola 5\nEsta cola se encuentra vacia\n\n");

	fclose(f);
	return 0;

}

/*
- writeSalida: se encarga de escribir en el archivo de salida los de una cola.
- Parametros de entrada: apuntador a la cola 
- Parametros de salida: 
*/
void writeSalida(COLA *q, FILE *f){

	NODO *first = q->primero;
	fprintf(f, "Cola %d\n", q->nro);
	fprintf(f, "PID\tEstado\tTiempo\tComando\n");
	fprintf(f, "%li\t%c\t%.2f\t%s\n", first->proceso->PID, first->proceso->Estado, first->proceso->Time, first->proceso->Comando);
	
	NODO *proximo = first->next;

	while(proximo != NULL){
		fprintf(f, "%li\t%c\t%.2f\t%s\n", proximo->proceso->PID, proximo->proceso->Estado, proximo->proceso->Time, proximo->proceso->Comando);
		proximo = proximo->next;
	}

	fprintf(f, "\n");

}

/*
- ProcEnEJec: permite obtener el apuntador al proceso que se encuentra en ejecu-
ción.
- Parametros de entrada: apuntador a la estructura de colas.
- Parametros de salida: objeto tipo Nodo que contiene el proceso en ejecución.
*/
NODO *ProcEnEjec(EstrucSched *s){

	COLA *cola = s->enEjecucion;

	NODO* tmp = cola->primero;

	while(tmp != NULL){
		if (tmp->proceso->Estado == 'E') {
			return tmp;
		}
		tmp = tmp->next;
	}
	return(0);
}
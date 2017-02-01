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

/*
	Este es el programa principal.
*/

#include "pscheduler.h"

int main(int argc, char const *argv[])
{
	char *filename;
	strcpy(filename, argv[1]);

	EstrucSched *estructura = Construye(filename);

	Imprime(estructura);

	return 0;
}

EstrucSched* Construye(char *filename){

	EstrucSched *E = (EstrucSched *) malloc(sizeof(EstrucSched));
	init_EstrucSched(E);

	long PID;
	char Estado;
	short Prioridad;
	float Time;
	char Comando[100];

	FILE *fp = fopen(filename, "r");

	while(fscanf(fp, "%li %c %hi %f %s", &PID, &Estado, &Prioridad, &Time, Comando) != EOF){

		printf("%li\t", PID);
		printf("%c\t", Estado);
		printf("%hi\t", Prioridad);
		printf("%.2f\t", Time);		 // .2 cantidad de decimales
		printf("%s\n", Comando);
		
		Proceso *p = (Proceso *) malloc(sizeof(Proceso));
		init_proceso(p, PID, Time, Estado, Comando);
		InsertarProceso(E, p, Prioridad);
	}

	return E;
	
}

void InsertarProceso(EstrucSched* s, Proceso *p, short prioridad){
	
	switch(prioridad){
		case '0':
			insertarProc(s->q0, p);
		case '1':
			insertarProc(s->q1, p);
		case '2':
			insertarProc(s->q2, p);
		case '3':
			insertarProc(s->q3, p);
		case '4':
			insertarProc(s->q4, p);
		case '5':
			insertarProc(s->q5, p);
	}
}

void insertarProc(COLA *q, Proceso *p){
	
	NODO *n = (NODO *) malloc(sizeof(NODO));
	init_nodo(n, p);
	
	if(!q->primero){
		q->primero = n;
		q->ultimo = n;
	}

	else{
		q->ultimo->next = (NODO *) n;
		q->ultimo = n;
	}

	q->size++;
}

/* Devuelve el proximo proceso a planificar.
   La rutina comienza buscando por la cola de mayor prioridad (q0) un proceso 
   que este en estado Listo. Si es encontrado, se coloca al final de su cola y se
   cambia su estado a En Ejecucion. Si ya existe un proceso en ejecucion, este 
   es detenido para planificar el otro. Si no encuentra ningun proceso,
   devuelve NULL.

  Ejemplo de un proceso detenido para planificar otro:
  	q0: E(0) L(1) L(2) L(3) **
  	ProxProceso(s)
  	q0: L(0) L(2) L(3) E(1)

  ** Se supone que los procesos 1, 2 y 3 fueron insertados a q0 mediante la interfaz
  de usuario. El proceso 0 teoricamente siempre fue el ultimo de la cola.
*/
Proceso *ProxProceso(EstrucSched *s){
	
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

Proceso *ProxProc(EstrucSched *s, COLA *q){

	NODO *n = q->primero;
	Proceso *p = n->proceso;

	// Teoricamente si los elementos de la cola son más de 1 y el primero 
	// "no esta listo", el segundo lo esta.
	// Ejemplo: E(0) L(1) L(2) -> L(0) L(2) E(1)
	// Ejemplo: L(0) L(1) L(2) -> L(1) L(2) E(0)
	// Ejemplo: L(0) -> E(0) 
	
	if(n != NULL && p->Estado == 'L'){
		// CAMBIAR EL ESTADO CON LA FUNCION CambiarEstado?
		p->Estado = 'E';
		q->primero = n->next;
		q->ultimo->next = n;
		q->ultimo = n;
		s->enEjecucion = n;
		return p;
	}

	return NULL;
}

void Imprime(EstrucSched *s){
	
	if(s->q0->size != 0) Imprimir(s->q0);

	if(s->q1->size != 0) Imprimir(s->q1);		

	if(s->q2->size != 0) Imprimir(s->q2);
		
	if(s->q3->size != 0) Imprimir(s->q3);

	if(s->q4->size != 0) Imprimir(s->q4);

	if(s->q5->size != 0) Imprimir(s->q5);
}

void Imprimir(COLA *q){

	NODO *first = q->primero;
	printf("Cola %d", q->nro);
	printf("PID\tEstado\tTiempo\tComando\n");
	printf("%li\t%c\t%f\t%s\n", first->proceso->PID, first->proceso->Estado, first->proceso->Time, first->proceso->Comando);
	
	NODO *proximo = first->next;

	while(proximo != NULL){
		printf("%li\t%c\t%f\t%s\n", proximo->proceso->PID, proximo->proceso->Estado, proximo->proceso->Time, proximo->proceso->Comando);
		proximo = proximo->next;
	}
}

void init_proceso(Proceso *p, long procesoid, float time, char estado, char* comando){
	p->PID = procesoid;
	p->Time = time;
	p->Estado = estado;
	strcpy(p->Comando, comando);
}

void init_nodo(NODO *n, Proceso *p){
	n->proceso = p;
	n->next = NULL;
}

void init_cola(COLA *q, int numero){
	q->primero = NULL;
	q->ultimo = NULL;
	q->size = 0;
	q->nro = numero;
}

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

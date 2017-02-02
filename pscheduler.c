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

int main(int argc, char *argv[])
{

	// Verificación de que el comando para ejecutar el programa sea correcto
	if (argc != 3){
	
		printf("Uso: pscheduler archivo_entrada archivo_salida\n");
		exit(-1);
	}

	// Creamos estructura de datos
	EstrucSched *estructura = Construye(argv[1]);
	
	int opcion;

	do{
		// Falta eliminar proceso en ejecucion
		printf("\t\t\tPSCHEDULER\n");
		printf("Bienvenido al planificador de procesos: Indique la opcion que desea\n");
		printf("1 ~ Insertar Proceso\n");
		printf("2 ~ Eliminar Proceso\n");
		printf("3 ~ Cambiar estado del Proceso\n");
		printf("4 ~ Mostrar el contenido del pscheduler\n");
		printf("5 ~ Salir\n");
		printf("Tu opcion: ");
		scanf("%d", &opcion);

		if(opcion>5 || opcion<1){
			printf("Su opcion debe estar 1 y 5\n");
			break;
		}

		switch(opcion){
		
			case 1:
				long PID;
				char Estado;
				short Prioridad;
				float Time;
				char Comando[100];

				printf("Introduzca en el orden: PID  prioridad  tiempo_ejecucion comando");
				scanf("%li %hi %f %s", &PID, &Prioridad, &Time, Comando);

				Proceso *p = (Proceso *) malloc(sizeof(Proceso));
				init_proceso(p, PID, Time, 'L', Comando);
				InsertarProceso(estructura, p, Prioridad);

				printf("Se inserto proceso correctamente\n");
				break;
			
			case 2:
				printf("Introduzca: PID  prioridad");
				scanf("%hi %li", &PID, &Prioridad);
				break;

			case 3:
				printf("Usted selecciono cambiar");
				break;

			case 4:
				printf("Usted selecciono mostrar");
				Imprime(estructura);
				break;

			case 5:
				printf("Llamada a la funcion de archivo salida");
				exit(0);

			default:
				scanf("%*s");
				printf("Su opcion debe estar 1 y 5\n");
				break;
			}

	}while(opcion !=5);

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

		Proceso *p = (Proceso *) malloc(sizeof(Proceso));
		init_proceso(p, PID, Time, Estado, Comando);
		InsertarProceso(E, p, Prioridad);
	}

	return E;
	
}

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

/** 
 *  Elimina el proceso que este en ejecucion actualmente.
 *
 *	Ejemplo de eliminacion de un proceso en ejecucion en el que luego se de
 *  ponerse en ejecucion, se insertaron 3 procesos:
 *	E(0) L(1) L(2) L(3)
 *  L(1) L(2) L(3)
 *
 *	Ejemplo:
 *	L(0) L(1) L(2) L(3)
 *	L(1) L(2) L(3) E(0)
 *
 *	Ejemplo: <--- Este caso FALLA!
 *	L(0) L(1) L(2)
 *  ProxProceso()
 *	L(1) L(2) E(0)
 *  InsertarProceso()
 *  L(1) L(2) E(0) L(3)
 *  ElimProcesoE()
 *  L(1) L(2) L(3)
 *
 *  HINT: Guiar la busqueda en base al estado de los procesos y no en base a prpx
 *  ademas guardar siempre el anterior el actual y el proximo para conectar el
 *  anterior con el proximo
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

/** 
 *  Devuelve el proximo proceso a planificar.
 *  La rutina comienza buscando por la cola de mayor prioridad (q0) un proceso 
 *  que este en estado Listo. Si es encontrado, se coloca al final de su cola y se
 *  cambia su estado a en ejecucion. Si ya existe un proceso en ejecucion o no 
 *  se encuentra ningun proceso, se retornara el valor NULL.
 * 
 *  @param s Estructura con las seis colas.
 *  @return Proceso que es planificado 
 * 
 *  Ejemplo:
 *  q0: vacia
 * 	q1: L(0) L(1) L(2) L(3)
 * 	ProxProceso(s)
 * 	q0: vacia
 * 	q1: L(1) L(2) L(3) E(0)
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

void CambiarEstado(EstrucSched *s, Proceso* p, Estado newestado){
	p->Estado = newestado;
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
	printf("Cola %d\n", q->nro);
	printf("PID\tEstado\tTiempo\tComando\n");
	printf("%li\t%c\t%.2f\t%s\n", first->proceso->PID, first->proceso->Estado, first->proceso->Time, first->proceso->Comando);
	
	NODO *proximo = first->next;

	while(proximo != NULL){
		printf("%li\t%c\t%.2f\t%s\n", proximo->proceso->PID, proximo->proceso->Estado, proximo->proceso->Time, proximo->proceso->Comando);
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

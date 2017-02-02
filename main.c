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

int main(int argc, char *argv[])
{

	// Verificación de que el comando para ejecutar el programa sea correcto
	if (argc != 3){
	
		printf("Uso: pscheduler archivo_entrada archivo_salida\n");
		exit(-1);
	}

	// Creamos estructura de datos
	EstrucSched *estructura = Construye(argv[1]);
	
	// Inicializamos variables
	long PID;
	char Estado;
	short Prioridad;
	float Time;
	char Comando[100];
	char opcion;

	do{
		printf("\t\t\tPSCHEDULER\n");
		printf("Bienvenido al planificador de procesos: Indique la opcion que desea\n");
		printf("1 ~ Insertar Proceso\n");
		printf("2 ~ Eliminar Proceso\n");
		printf("3 ~ Eliminar Proceso en Ejecucion");
		printf("4 ~ Proximo Proceso a Planificar");
		printf("5 ~ Cambiar estado del Proceso\n");
		printf("6 ~ Mostrar el contenido del pscheduler\n");
		printf("7 ~ Salir\n");
		printf("Tu opcion: ");
		scanf("%c", &opcion);
		getchar();

			switch(opcion){
			
				case '1':
					printf("Insertar proceso");
					printf("Introduzca en el orden: PID  prioridad  tiempo_ejecucion comando");
					scanf("%li %hi %f %s", &PID, &Prioridad, &Time, Comando);

					Proceso *p = (Proceso *) malloc(sizeof(Proceso));
					init_proceso(p, PID, Time, 'L', Comando);
					InsertarProceso(estructura, p, Prioridad);

					printf("Se inserto proceso correctamente\n");
					break;
				
				case '2':
					printf("Eliminar proceso");
					printf("Introduzca: PID  prioridad");
					scanf("%hi %li", &PID, &Prioridad);
					ElimProceso(estructura, PID, Prioridad);

					printf("Se elimino proceso correctamente");
					break;

				case '3':
					printf("Eliminar proceso en ejecucion");
					ElimProcesoE(estructura);
					printf("Se elimino proceso en ejecucion correctamente");
					break;

				case '4':
					printf("Proximo proceso a planificar");
					ProxProceso(estructura);
					break;

				case '5':
					printf("Cambiar estado del proceso");
					printf("Introduzca si desea cambiar el estado a Listo (L) o en ejecucion (E)");
					scanf("%s", Estado);
					// No se que como incializar el proce
					CambiarEstado(estructura, Proceso* p, Estado)
					break;

				case '6':
					printf("ESTADO ACTUAL DE LA ESTRUCTURA DE COLAS\n");
					Imprime(estructura);

				case '7':
					printf("Antes de salir inidique el nombre del archivo de salida");
					char* archivo_salida;
					scanf("%s", archivo_salida)
					exit(0);

				default:
					//scanf("%*s");
					printf("Su opcion debe estar 1 y 7\n");
					break;
			}
		}

	}while(opcion !=8);

	return 0;
}
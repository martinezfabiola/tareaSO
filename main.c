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
*******************************************************************************/
#include "pscheduler.h"

int main(int argc, char *argv[])
{

	// Verificación de que el comando para ejecutar el programa sea correcto
	if (argc != 3){
	
		printf("--Uso: pscheduler archivo_entrada archivo_salida\n");
		exit(-1);
	}

	// Creamos estructura de datos
	EstrucSched *estructura = Construye(argv[1]);
	
	long PID;
	char Estado;
	short Prioridad;
	float Time;
	char Comando[100];
	char opcion;

	do{
		printf("\n\t\t\tPSCHEDULER\n");
		printf("Bienvenido al planificador de procesos: Indique la opcion que desea\n");
		printf("1 ~ Insertar proceso\n");
		printf("2 ~ Eliminar proceso\n");
		printf("3 ~ Eliminar proceso en ejecucion\n");
		printf("4 ~ Proximo proceso a planificar\n");
		printf("5 ~ Cambiar estado del proceso\n");
		printf("6 ~ Mostrar el contenido del pscheduler\n");
		printf("7 ~ Salir\n");
		printf("Tu opcion: ");
		scanf("%c", &opcion);
		getchar();

			switch(opcion){
			
				case '1':
					printf("\n--Introduzca en el orden: PID prioridad tiempo_ejecucion comando\n");
					scanf("%li %hi %f %s", &PID, &Prioridad, &Time, Comando);

					Proceso *p = (Proceso *) malloc(sizeof(Proceso));
					init_proceso(p, PID, Time, 'L', Comando);
					InsertarProceso(estructura, p, Prioridad);

					getchar();
					break;
				
				case '2':
					printf("\n--Introduzca los argumentos en el orden: PID prioridad\n");
					scanf("%li %hi", &PID, &Prioridad);

					ElimProceso(estructura, PID, Prioridad);

					getchar();
					break;

				case '3':
					ElimProcesoE(estructura);
					break;

				case '4':{
					Proceso *proc = ProxProceso(estructura);
					break;
				}
				case '5':{
					if (estructura->enEjecucion){
						char yn;
						printf("Se cambiara el proceso en ejecucion a listo\n");
						printf("Esta seguro de esto? s/n: ");
						scanf("%c", &yn);
						getchar();
						printf("\n");
						if(yn == 's'){
							Proceso *EnEjec = ProcEnEjec(estructura)->proceso;
							CambiarEstado(estructura, EnEjec, 'L');
							break;
						}
					}
					else{
						printf("Actualmente no hay ningun proceso en ejecucion\n");
						break;
					}
				}
				case '6':
					printf("\n--Estado actual de la estructura de colas\n");
					Imprime(estructura);
					break;

				case '7':
					Salida(estructura, argv[2]);
					printf("Se escribio el contenido del planificador en el archivo \"%s\"\n", argv[2]);
					printf("Hasta luego!\n");
					exit(0);

				default:
					printf("Su opcion debe estar entre los valores 1 y 7\n");
					getchar();
					break;
			}

	}while(opcion !=8);

	return 0;
}
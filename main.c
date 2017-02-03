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
	
		printf("Uso: pscheduler archivo_entrada archivo_salida\n");
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
		printf("1 ~ Insertar Proceso\n");
		printf("2 ~ Eliminar Proceso\n");
		printf("3 ~ Eliminar Proceso en Ejecucion\n");
		printf("4 ~ Proximo Proceso a Planificar\n");
		printf("5 ~ Cambiar estado del Proceso\n");
		printf("6 ~ Mostrar el contenido del pscheduler\n");
		printf("7 ~ Salir\n");
		printf("Tu opcion: ");
		scanf("%c", &opcion);
		getchar();

			switch(opcion){
			
				case '1':
					printf("\nIntroduzca en el orden: PID  prioridad  tiempo_ejecucion comando\n");
					scanf("%li %hi %f %s", &PID, &Prioridad, &Time, Comando);

					Proceso *p = (Proceso *) malloc(sizeof(Proceso));
					init_proceso(p, PID, Time, 'L', Comando);
					InsertarProceso(estructura, p, Prioridad);

					printf("\nProceso insertado correctamente\n");
					break;
				
				case '2':
					printf("\nIntroduzca los argumentos en el orden: PID prioridad\n");
					scanf("%hi %li", &PID, &Prioridad);
					ElimProceso(estructura, PID, Prioridad);

					printf("\nSe elimino proceso correctamente");
					break;

				case '3':
					ElimProcesoE(estructura);
					
					printf("\nSe elimino proceso en ejecucion correctamente");
					break;

				case '4':{
					Proceso *proc = ProxProceso(estructura);
					
					printf("\nEl proximo proceso a planificar es el de PID: %li", p->PID);								
					break;
				}
				case '5':
					printf("\nIntroduzca si desea cambiar el estado a Listo (L) o en ejecucion (E)");
					scanf("%s", &Estado);

					//CambiarEstado(estructura, Proceso* p, Estado)
					break;

				case '6':
					printf("\nEstado actual de la estructura de colas\n");
					Imprime(estructura);
					break;

				case '7':
					Salida(estructura, argv[2]);
					printf("\nHasta luego!\n");
					exit(0);

				default:
					printf("Su opcion debe estar entre los valores 1 y 7\n");
					break;
			}

	}while(opcion !=8);

	return 0;
}
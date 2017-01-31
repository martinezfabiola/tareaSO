#include <stdio.h>

int main(int argc, char const *argv[])

{

	int PID;
	char Estado;
	short Prioridad;
	float Time;
	char Comando[100];

	FILE *fp;
	fp = fopen(argv[1], "r");
	
	while(fscanf(fp, "%d %c %hi %f %s", &PID, &Estado, &Prioridad, &Time, Comando) != EOF){

			printf("%d\t", PID);
			printf("%c\t", Estado);
			printf("%hi\t", Prioridad);
			printf("%.2f\t", Time); // .2 cantidad de decimales
			printf("%s\n", Comando);

	}
}

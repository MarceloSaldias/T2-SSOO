#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include "../process/process.h"
#include "../queue/queue.h"
#include "../file_manager/manager.h"

// Parámetro del programa
int q;
char* output_file;


int main(int argc, char const *argv[])
{
	/*Lectura del input*/
	char *file_name = (char *)argv[1];
	InputFile *input_file = read_file(file_name);

	output_file = (char *)argv[2];
	q = atoi((char *) argv[3]); 

	printf("Archivo output: %s\n", output_file);
	printf("Valor de q: %d\n", q);

	/*Mostramos el archivo de input en consola*/
	printf("Nombre archivo: %s\n", file_name);
	printf("Cantidad de procesos: %d\n", input_file->len);
	printf("Procesos:\n");



	for (int i = 0; i < input_file->len; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			printf("%s ", input_file->lines[i][j]);
		}
		printf("\n");
	}
	
	// Acá comienzo la tarea !!

  // Traemos todos los procesos a una cola inicial, solo para almacenar los datos iniciales
	Queue* initial_q = queue_init(0);

	for (int i = 0; i < input_file->len; ++i)
	{
		Process* new_process = process_init_array(input_file->lines[i]);
		queue_append(initial_q, new_process);
	}

	// Podemos verificar que están los mismos procesos que se printean arriba
	queue_print(initial_q);

	// Cola de mayor prioridad
	Queue* high_prio_q = queue_init((int) 2 * q);
	// Cola de media prioridad
	Queue* mid_prio_q = queue_init((int) 1 * q);
	// Cola de baja prioridad
	Queue* low_prio_q = queue_init(0);

	// Tiempo de la simulación
	//int curr_time = 0;

	queue_destroy(initial_q);
	queue_destroy(high_prio_q);
	queue_destroy(mid_prio_q);
	queue_destroy(low_prio_q);
	input_file_destroy(input_file);
}
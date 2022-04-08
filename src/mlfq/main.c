#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include "../process/process.h"
#include "../queue/queue.h"
#include "../file_manager/manager.h"

// Parámetros del programa
int q;
char* output_file;

// Declaración de colas
Queue* initial_q;
Queue* high_prio_q;
Queue* mid_prio_q;
Queue* low_prio_q;
Queue* finished_q;

// Tiempo de simulación
int t = 0;
// Proceso actualmente en CPU
Process* cpu_process;
// Quantum restante en la CPU
int curr_quantum = -1;

void update_running_process()
{
	if (!cpu_process) return;
	cpu_process -> curr_wait -= 1;
	cpu_process -> cycles -= 1;
	curr_quantum -= 1;
	// Si terminó su ejecución
	if (cpu_process -> cycles == 0)
	{
		cpu_process -> status = FINISHED;
		cpu_process -> turnaround_time = t - cpu_process -> start_time;
		// Lo añadimos a la cola de procesos terminados
		queue_append(finished_q, cpu_process);
		cpu_process = NULL;
	}
	else if (cpu_process -> curr_wait == 0)
	{
		cpu_process -> status = WAITING;
		// Aumenta su prioridad si está en la cola 2 (cola 1 no puede auemntar)
		if (cpu_process -> priority == 2)
		{
			// Aumentamos su prioridad
			cpu_process -> priority = 1;
			queue_append(high_prio_q, cpu_process);
			cpu_process = NULL;
		}
		// Si está en la cola 3, entonces vuelve a la cola 3
		if (cpu_process -> priority == 3)
		{
			queue_append(low_prio_q, cpu_process);
			cpu_process = NULL;
		}
	}
	else if (cpu_process -> curr_wait > 0 && curr_quantum == 0 && cpu_process -> priority != 3)
	{
		cpu_process -> times_interrupted += 1;
		// Se reduce la prioridad del proceso
		if (cpu_process -> priority == 1)
		{
			cpu_process -> priority = 2;
			queue_append(mid_prio_q, cpu_process);
			cpu_process = NULL;
		}
		else if (cpu_process -> priority == 2)
		{
			cpu_process -> priority = 3;
			queue_append(low_prio_q, cpu_process);
			cpu_process = NULL;
		}
	}
}

int main(int argc, char const *argv[])
{
	/*Lectura del input*/
	char *file_name = (char *)argv[1];
	InputFile *input_file = read_file(file_name);

	output_file = (char *)argv[2];
	q = atoi((char *) argv[3]); 

	printf("Cantidad de procesos: %d\n", input_file->len);
	
	// ========================= INICIO TAREA =========================
	// Inicializamos las colas
	initial_q = queue_init(0);
	high_prio_q = queue_init((int) 2 * q);
	mid_prio_q = queue_init((int) 1 * q);
	low_prio_q = queue_init(0);
	finished_q = queue_init(0);

  	// Traemos todos los procesos a una cola inicial,
	// solo para almacenar los procesos antes de entrar al MLFQ
	for (int i = 0; i < input_file->len; ++i)
	{
		Process* new_process = process_init_array(input_file->lines[i]);
		queue_append(initial_q, new_process);
	}

	// Tiempo de la simulación
	//int curr_time = 0;

	queue_destroy(initial_q);
	queue_destroy(high_prio_q);
	queue_destroy(mid_prio_q);
	queue_destroy(low_prio_q);
	queue_destroy(finished_q);
	input_file_destroy(input_file);
}
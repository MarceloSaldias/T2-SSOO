#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include "../process/process.h"
#include "../queue/queue.h"
#include "../file_manager/manager.h"
#include <unistd.h>
//#include <windows.h>
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
// Flag para ver si le toca aging mientras está en ejecución
int running_aging = 0;

void update_running_process()
{
	// Si no hay ningun proceso corriendo
	// 	nos saltamos esta sección
	if (!cpu_process) return;

	cpu_process -> status = RUNNING;
	cpu_process -> curr_wait -= 1;
	cpu_process -> cycles -= 1;
	curr_quantum -= 1;

	// Verificamos si el proceso en la CPU tuvo aging
	//	solo si no lo presentó en un ciclo anterior
	if (!running_aging)
	{
		running_aging = cpu_aging(cpu_process, t);
	}

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
		cpu_process -> curr_wait = cpu_process -> wait;
		// Si está en la cola 1, 2 o tuvo su aging mientras estuvo en ejecución
		if (cpu_process -> priority == 2 || cpu_process -> priority == 1 || running_aging == 1)
		{
			cpu_process -> priority = 2;
			queue_append(high_prio_q, cpu_process);
			cpu_process = NULL;
			running_aging = 0;
		}
		// Si está en la cola 3, entonces vuelve a la cola 3
		else if (cpu_process -> priority == 0)
		{
			queue_append(low_prio_q, cpu_process);
			cpu_process = NULL;
		}
	}
	// Si me quedé sin quantum de ejecución y estoy en las colas 1 o 2
	else if (cpu_process -> priority != 0 && cpu_process -> curr_wait > 0 && curr_quantum == 0)
	{
		cpu_process -> times_interrupted += 1;
		cpu_process -> status = READY;
		// Si tuvo su aging en ejecución, pasa a la cola de alta prioridad
		if (running_aging == 1)
		{
			cpu_process -> priority = 2;
			queue_append(high_prio_q, cpu_process);
			cpu_process = NULL;
			running_aging = 0;
		}
		// Se reduce la prioridad del proceso
		else if (cpu_process -> priority == 2)
		{
			cpu_process -> priority = 1;
			queue_append(mid_prio_q, cpu_process);
			cpu_process = NULL;
		}
		else if (cpu_process -> priority == 1)
		{
			cpu_process -> priority = 0;
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

	/*
	Por cada unidad de tiempo el scheduler debe realizar:
	1. Actualizar los procesos que cumplan su I/O burst de WAITING a READY. CHECK
	2. En caso de existir un proceso en estado RUNING, actualizar su estado segun corresponda. CHECK
	3. Ingresar los procesos a sus colas correspondientes siguiendo la orden de ingreso. CHECK
	3.1) Si un proceso salio de la CPU, ingresarlo a la cola correspodiente. CHECK
	3.2) Por cada proceso p comprobar si t = t iniciop e ingresarlo a la primera cola. CHECK
	3.3) Por cada proceso p en la segunda cola verificar si se cumple (t − t iniciop) % Sp = 0 e ingresarlo a la
	primera cola. CHECK
	3.4) Por cada proceso p en la tercera cola verificar si se cumple (t − t iniciop) % Sp = 0 e ingresarlo a la
	primera cola. CHECK
	4. Ingresar un proceso a la CPU si corresponde, esto implica cambiar su estado de READY a RUNNING. CHECK 
	*/

	while (finished_q -> count != input_file -> len)
	{
		// Actualizar los procesos que cumplan su I/O burst de WAITING a READY.
		queue_update_waiting(high_prio_q);
		queue_update_waiting(mid_prio_q);
		queue_update_waiting(low_prio_q);

		//En caso de existir un proceso en estado RUNNING, actualizar su estado segun corresponda.
		update_running_process();

		// Ingresamos procesos a la cola de alta prioridad dependiendo de su start_time
		queue_start_time(initial_q, t, high_prio_q);

		queue_aging(mid_prio_q, t, high_prio_q);
		queue_aging(low_prio_q, t, high_prio_q);

		// ------------ ACÁ DEFINO CUAL PROCESO ENTRA EN LA CPU ------------------
		// primero debo recorrer la cola high, luego la mid y luego la low
		// si hay uno listo en la cola 1, ingresarlo
		int new_cpu_process = 0;
		if (!cpu_process)
		{
			cpu_process = queue_fifo(high_prio_q);
			if (cpu_process)
			{
				new_cpu_process = 1;
				curr_quantum = high_prio_q -> quantum;
			}
		}
		// si no hay uno listo en la cola 1 y si hay uno listo en la cola 2, ingresarlo
		if (!cpu_process)
		{
			cpu_process = queue_fifo(mid_prio_q);
			if (cpu_process)
			{
				new_cpu_process = 1;
				curr_quantum = mid_prio_q -> quantum;
			}
		}
		// si no hay uno listo en la cola 2 y si hay uno listo en la cola 3, ingresarlo
		if (!cpu_process)
		{
			// SJF siempre encuentra uno, y los empates los saca por FIFO por como está implementado
			cpu_process = queue_sjf(low_prio_q);
			if (cpu_process)
			{
				new_cpu_process = 1;
				curr_quantum = 0;
			}
		}
		if (new_cpu_process)
		{	
			if (cpu_process -> times_chosen_by_cpu == 0)
			{
				cpu_process -> response_time = t - cpu_process -> start_time;
			}
			cpu_process -> times_chosen_by_cpu += 1;
		}

		queue_waiting_time_on_ready(high_prio_q);
		queue_waiting_time_on_ready(mid_prio_q);
		queue_waiting_time_on_ready(low_prio_q);

		sleep(0.1);
		t++;
	}

	FILE *file_output = fopen(output_file, "w");
	for (int i = 0; i < finished_q -> count; i++)
	{
		Process* p = queue_get(finished_q, i);
		fprintf(file_output, "%s,%d,%d,%d,%d,%d\n", p->name, p->times_chosen_by_cpu, p->times_interrupted, p->turnaround_time, p->response_time, p->waiting_time);
	}
	fclose(file_output);

	queue_destroy(initial_q);
	queue_destroy(high_prio_q);
	queue_destroy(mid_prio_q);
	queue_destroy(low_prio_q);
	queue_destroy(finished_q);
	input_file_destroy(input_file);
}
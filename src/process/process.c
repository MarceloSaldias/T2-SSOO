// Libreria estandar de c
#include <stdlib.h>
// Liberia de input y output
#include <stdio.h>
// Libreria de Strings
#include <string.h>

#include "process.h"

/** Constructor de un proceso */
Process* process_init(int pid, char name[33], int priority, enum Status status, int aging)
{
  Process* process = malloc(sizeof(Process));
  
  // Inicializamos el proceso
  process -> pid = pid;
  strcpy(process -> name, name);
  process -> priority = priority;
  // TODO: Ver si esto se define altiro o en tiempo de ejecución.
  process -> start_time = 0;
  process -> cycles = 0;
  process -> wait = 0;
  process -> status = status;
  process -> aging = aging;

  return process;
}

/** Constructor de un proceso con array */
Process* process_init_array(char** args)
{
  Process* process = malloc(sizeof(Process));
  
  // Inicializamos el proceso
  process -> name = args[0];
  process -> pid = atoi(args[1]);
  process -> start_time = atoi(args[2]);
  process -> cycles = atoi(args[3]);
  process -> wait = atoi(args[4]);
  process -> curr_wait = atoi(args[4]);
  process -> waiting_delay = atoi(args[5]);
  process -> curr_waiting_delay = atoi(args[5]);
  process -> aging = atoi(args[6]);
  process -> status = READY;
  process -> priority = 2;

  // Inicializamos los datos de analítica
  process -> times_chosen_by_cpu = 0;
  process -> times_interrupted = 0;
  process -> turnaround_time = 0;
  process -> response_time = 0;
  process -> waiting_time = 0;

  return process; 
}

/** Funcion que destruye el proceso la memoria utilizada */
void process_destroy(Process* process)
{
  // Liberamos la memoria utilizada para almacenar el struct
  free(process);
}

/** Funcion para imprimir un proceso */
void process_print(Process* process)
{
  printf("Proceso %s (PID: %d) Start Time: %d\n", process->name, process->pid, process->start_time);
}
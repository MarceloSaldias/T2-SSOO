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

/** Funcion que destruye el proceso la memoria utilizada */
void process_destroy(Process* process)
{
  // Liberamos la memoria utilizada para almacenar el struct
  free(process);
}
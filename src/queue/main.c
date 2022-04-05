/** Módulo estándar de Input y Output */
#include <stdio.h>
/** Módulo estándar de números enteros */
#include <stdint.h>
/** Módulo estándar de valores booleanos */
#include <stdbool.h>

// Archivo de colas
#include "queue.h"
// Archivo de procesos
#include "../process/process.h"

int main()
{
  // En esta sección crearemos una lista ligada a partir de sus constructor
  // y luego la usaremos normalmente. La struct LinkedList esta definida en el
  // archivo linked_list.h junto con todas sus funciones publicas. En le archivo
  // linked_list.c esta el codigo de todas las funciones publicas y pricadas

  // Creo la cola con la prioridad
  Queue* q = queue_init(2);

  // Agrego 10 procesos a la cola 
  for (int i = 0; i < 10; i++)
  {
    // TODO: Funciones de creación de procesos
    queue_append(q, i);
  }

  // Imprimo el proceso de la posicion 5
  Process pos_5 = queue_get(queue, 5);
  printf("El nombre del proceso en la posicion %d es %s\n", 5, pos_5 -> name);

  // Destruyo la cola liberando todos sus recursos
  queue_destroy(q);
  
  return 0;
}

// Libreria estandar de c
#include <stdlib.h>
// Liberia de input y output
#include <stdio.h>

// Importamos el archivo .h correspondiente
#include "queue.h"
// Importamos las definiciones de process
#include "../process/process.h"

int main()
{
  // En esta sección crearemos una lista ligada a partir de sus constructor
  // y luego la usaremos normalmente. La struct LinkedList esta definida en el
  // archivo linked_list.h junto con todas sus funciones publicas. En le archivo
  // linked_list.c esta el codigo de todas las funciones publicas y pricadas

  // Creo la cola con la prioridad
  Queue* q = queue_init(2);

  Process* p1 = process_init(1, "Test", 2, RUNNING, 2);
  Process* p2 = process_init(2, "Test2", 2, RUNNING, 2);
  Process* p3 = process_init(3, "Test3", 2, RUNNING, 2);

  queue_append(q, p1);
  queue_append(q, p2);
  queue_append(q, p3);
  
  Process* pos_0 = queue_get(q, 0);
  Process* pos_1 = queue_get(q, 1);
  Process* pos_2 = queue_get(q, 2);

  printf("El nombre del proceso en la posición 0 es %s\n", pos_0->name);
  printf("El nombre del proceso en la posición 1 es %s\n", pos_1->name);
  printf("El nombre del proceso en la posición 2 es %s\n", pos_2->name);

  process_destroy(p1);
  process_destroy(p2);
  process_destroy(p3);
  // Destruyo la cola liberando todos sus recursos
  queue_destroy(q);
  
  return 0;
}
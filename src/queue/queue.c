// Libreria estandar de c
#include <stdlib.h>
// Liberia de input y output
#include <stdio.h>

// Importamos el archivo .h correspondiente
#include "queue.h"
// Importamos las definiciones de process
#include "../process/process.h"

//////////////////////////// Funciones privadas ///////////////////////////

// Las funciones que tienen el keyword "static" solo pueden ser llamadas desde
// el archivo en el que fueron definidas. Sirve por ejemplo para hacer funciones
// internas ajenas al uso que se le va a dar a la estructura.
// Por ejemplo la funcion node_init solo se va a usar al agregar elementos a la
// lista ligada pero no se va a llamar desde el programa principal

/** Funcion que crea un nodo de la cola a partir de un valor */
static Node* node_init(Process value)
{
  // Pido la memoria para el nodo
  Node* node = malloc(sizeof(Node));

  // Inicializo los referencias en NULL
  node -> last = NULL;
  node -> next = NULL;

  // Le doy el valor correspondiente
  node -> value = value;

  // Retorno el nodo
  return node;
}

/** Funcion que libera recursivamente la memoria de la lista ligada */
static void nodes_destroy(Node* node)
{
  // Si el nodo no es NULL, llama recursivamente al siguiente
  // y libero el nodo actual
  if (node)
  {
    nodes_destroy(node -> next);
    free(node);
  }
}

//////////////////////////// Funciones publicas ///////////////////////////

/** Constructor de una cola. La crea vacia inicialmente */
Queue* queue_init(int quantum)
{
  // Primero pido la memoria para la lista ligada
  Queue* queue = malloc(sizeof(Queue));

  // Luego inicializo en 0 el contador de procesos y en NULL las referencias
  queue -> count = 0;
  queue -> start = NULL;
  queue -> end = NULL;
  // Definimos el quantum de la cola
  queue -> quantum = quantum;

  // Retorno la cola
  return queue;
}

/** Funcion que agrega un elemento al final de la cola */
void queue_append(Queue* queue, Process process)
{
  // Primero creo un nodo nuevo
  Node* node = node_init(process);

  // Luego lo agrego a la cola
  if (!queue -> count)
  {
    // En el caso de que este vacia la cola dejo el nodo como inicial y final
    queue -> start = node;
    queue -> end = node;
  }
  else
  {
    // En otro caso
    // Lo conecto con el ultimo nodo de la cola
    queue -> end -> next = node;
    // Guardo la referencia al ex-ultimo nodo
    node -> last = queue -> end;
    // Hago el nuevo nodo el ultimo nodo de la cola
    queue -> end = node;
  }

  // Sumo 1 al numero de nodos
  queue -> count++;
}

/** Funcion que obtiene el valor de la cola en la posicion dada */
Process queue_get(Queue* queue, int position)
{
  // Si no hay suficientes nodos, hago un error
  if (position >= queue -> count)
  {
    printf("Error, el indice al que estas accediendo supera el largo de la cola\n");
¡    exit(1);
  }

  // Me muevo por los nodos hasta encontrar la posicion que busco
  Node* actual = queue -> start;
  for (int i = 0; i < position; i++)
  {
    actual = actual -> next;
  }

  // Retoro el valor del nodo correspondiente
  return actual -> value;
}

/** Funcion que destruye la lista ligada liberando la memoria utilizada */
void queue_destroy(Queue* queue)
{
  // Primero libero la memoria de todos los nodos de manera recursiva
  nodes_destroy(queue -> start);

  // Luego libero la memoria de la lista
  free(queue);
}

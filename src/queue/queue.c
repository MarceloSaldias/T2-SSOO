// Libreria estandar de c
#include <stdlib.h>
// Liberia de input y output
#include <stdio.h>
// INFINITY
#include <math.h>

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
static Node* node_init(Process* value)
{
  // Pido la memoria para el nodo
  Node* node = malloc(sizeof(Node));

  // Inicializo los referencias en NULL
  node -> last = NULL;
  node -> next = NULL;

  // Le doy el valor correspondiente
  node -> process = value;

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
    process_destroy(node->process);
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
void queue_append(Queue* queue, Process* process)
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
Process* queue_get(Queue* queue, int position)
{
  // Si no hay suficientes nodos, hago un error
  if (position >= queue -> count)
  {
    printf("Error, el indice al que estas accediendo supera el largo de la cola\n");
    exit(1);
  }

  // Me muevo por los nodos hasta encontrar la posicion que busco
  Node* actual = queue -> start;
  for (int i = 0; i < position; i++)
  {
    actual = actual -> next;
  }

  // Retoro el valor del nodo correspondiente
  return actual -> process;
}

/** Funcion que obtiene el valor de la cola en la posicion dada y lo saca de la cola */
Process* queue_pop(Queue* queue, int position)
{
  // Si no hay suficiente nodos, tiro error
  if (position >= queue -> count)
  {
    printf("Error, el indice al que estas accediendo supera el largo de la cola\n");
    exit(1);
  }

  // Declaramos el proceso y nodo que vamos a seleccionar
  Process* selected_process;
  Node* selected_node;

  // Si tengo solamente un nodo
  if (queue -> count == 1)
  {
    // Guardamos el nodo
    selected_node = queue -> start;
    // Guardamos el proceso
    selected_process = selected_node -> process;
    // Liberamos la memoria usada por el nodo
    free(selected_node);
    // Quitamos la referencia al nodo en la cola
    queue -> start = NULL;
    queue -> end = NULL;
  }
  // Es el primer nodo
  else if (position == 0)
  {
    // Guardamos el nodo
    selected_node = queue -> start;
    // Guardamos el proceso
    selected_process = selected_node -> process;
    // El comienzo de la cola es el siguiente nodo
    queue -> start = selected_node -> next;
    // Quitamos la referencia del siguiente nodo
    selected_node -> next -> last = NULL;
    // Liberamos la memoria usada por el nodo
    free(selected_node);
  }
  // Es el último nodo
  else if (position == queue -> count - 1)
  {
    // Guardamos el nodo
    selected_node = queue -> end;
    // Guardamos el proceso
    selected_process = selected_node -> process;
    // El fin de la cola es el nodo anterior
    queue -> end = selected_node -> last;
    // Quitamos la referencia del nodo anterior
    selected_node -> last -> next = NULL;
    // Liberamos la memoria usada por el nodo
    free(selected_node);
  }
  // Es algún nodo entremedio
  else
  {
    for (int i = 0; i < position; i++)
    {
      // Me muevo por los nodos hasta encontrar la posicion que busco
      selected_node = queue -> start;
      for (int i = 0; i < position; i++)
      {
        selected_node = selected_node -> next;
      }
      // Guardamos el proceso que queremos sacar
      selected_process = selected_node -> process;

      // Mapeamos los nodos a cada lado
      Node* node_before = selected_node -> last;
      Node* node_after = selected_node -> next;

      node_before -> next = node_after;
      node_after -> last = node_before;

      // Liberamos la memoria usada por el nodo
      free(selected_node);
    }
  }
  queue -> count -= 1;
  return selected_process;
}

/** Funcion que destruye la lista ligada liberando la memoria utilizada */
void queue_destroy(Queue* queue)
{
  // Primero libero la memoria de todos los nodos de manera recursiva
  nodes_destroy(queue -> start);

  // Luego libero la memoria de la lista
  free(queue);
}

/** Imprime información de los elementos de la cola*/
void queue_print(Queue* queue)
{
  if (queue -> count == 0) return;
  for (int i = 0; i < queue->count; i++)
  {
    Process* curr_process = queue_get(queue, i);
    if (i != 0) printf(" -> ");
    printf("[%d] %s <%d/%d>", curr_process->status, curr_process->name, curr_process->wait, curr_process->cycles);
  }
  printf("\n");
}

/** Obtiene un proceso de la lista por FIFO */
Process* queue_fifo(Queue* queue)
{
  Process* selected_process;
  for (int i = 0; i < queue -> count; i++)
  {
    selected_process = queue_get(queue, i);
    if (selected_process -> status == READY)
    {
      return queue_pop(queue, i);
    }
  }
  return NULL;
}

/** Obtiene un proceso de la lista por SJF, empates por FIFO */
Process* queue_sjf(Queue* queue)
{
  int best_process_index = -1;
  int best_time = (int) INFINITY;
  // Flag para retornar
  int selected_once = 0;
  for (int i = 0; i < queue -> count; i++)
  {
    Process* current_process = queue_get(queue, i);
    if (current_process -> status == READY && current_process -> curr_wait < best_time)
    {
      best_time = current_process -> curr_wait;
      best_process_index = i;
      selected_once = 1;
    }
  }
  if (selected_once) return queue_pop(queue, best_process_index);
  return NULL;
}

/** Obtiene los indices de los procesos que tienen su start_time = current_time */
void queue_start_time(Queue* from_queue, int current_time, Queue* to_queue)
{
  if (from_queue -> count == 0) return;
  int checked_incomplete = 1;
  while (checked_incomplete)
  {
    for (int i = 0; i < from_queue -> count; i++)
    {
      if (i == from_queue -> count - 1)
      {
        checked_incomplete = 0;
      }
      Process* p = queue_get(from_queue, i);
      if (p -> start_time == current_time)
      {
        queue_append(to_queue, queue_pop(from_queue, i));
        break;
      }
    }
  }
}


/** Mueve los procesos de from_queue que tienen su (current_time - start_time) % aging = 0 a to_queue */
void queue_aging(Queue* from_queue, int current_time, Queue* to_queue)
{
  if (from_queue -> count == 0 ) return;

  int checked_incomplete = 1;
  while (checked_incomplete)
  {
    for (int i = 0; i < from_queue -> count; i++)
    {
      if (i == from_queue -> count - 1)
      {
        checked_incomplete = 0;
      }
      Process* p = queue_get(from_queue, i);
      if ((current_time - p->start_time) % p->aging == 0)
      {
        queue_append(to_queue, queue_pop(from_queue, i));
        break;
      }
    }
  }
}

/** Entrega 1 si el proceso debe realizar aging en t = current_time y 0 en otro caso */
int cpu_aging(Process* p, int current_time)
{
  if ((current_time - p->start_time) % p->aging == 0)
  {
    return 1;
  }
  return 0;
}

/** Actualiza el estado de WAITING a READY de los procesos en la cola si completaron su tiempo de espera */
void queue_update_waiting(Queue* queue)
{
  for (int i = 0; i < queue -> count; i++)
  {
    Process* p = queue_get(queue, i);
    if (p -> status == WAITING)
    {
      p->waiting_time += 1;
      p->curr_waiting_delay -= 1;
      if (p->curr_waiting_delay == 0)
      {
        p->status = READY;
        p->curr_waiting_delay = p->waiting_delay;
      }
    }
  }
}

/** Aumenta el contador de waiting_time de todos los proceso en estado READY */
void queue_waiting_time_on_ready(Queue* queue)
{
  for (int i = 0; i < queue -> count; i++)
  {
    Process* p = queue_get(queue, i);
    if (p -> status == READY)
    {
      p->waiting_time += 1;
    }
  }
}
#pragma once

// Archivo con definiciones de proceso
#include "../process/process.h"

/** Estructura de un nodo de la cola. Mantiene una referencia al nodo
siguiente y al nodo anterior ademas de mantener un numero */
typedef struct node
{
  /** Puntero al nodo anterior */
  struct node* last;
  /** Puntero al nodo siguiente */
  struct node* next;
  /** Numero almacenado en el nodo */
  Process* process;
} Node;

/** Estructura de una lista ligada. Referencia a los extremos y mantiene un
contador de nodos en la lista */
typedef struct queue
{
  /** Nodo inicial de la cola */
  Node* start;
  /** Nodo final de la cola */
  Node* end;
  /** Contador de elementos de la lista ligada */
  int count;
  /** Quantum, debe ser definido en la inicialización **/
  int quantum;
} Queue;

///////////////////////////// Funcione publicas ///////////////////////////

/** Constructor de una cola. La crea vacia inicialmente */
Queue* queue_init(int quantum);

/** Funcion que agrega un elemento al final de la cola */
void queue_append(Queue* queue, Process* process);

/** Funcion que obtiene el valor de la cola en la posicion dada */
Process* queue_get(Queue* queue, int position);

/** Funcion que obtiene el valor de la cola en la posicion dada y lo saca de la cola */
Process* queue_pop(Queue* queue, int position);

/** Funcion que destruye la cola liberando la memoria utilizada */
void queue_destroy(Queue* queue);

/** Imprime información de los elementos de la cola*/
void queue_print(Queue* queue);

/** Obtiene un proceso de la lista por FIFO */
Process* queue_fifo(Queue* queue);

/** Obtiene un proceso de la lista por SJF */
Process* queue_sjf(Queue* queue);

/** Obtiene los indices de los procesos que tienen su start_time = current_time */
void queue_start_time(Queue* from_queue, int current_time, Queue* to_queue);

/** Obtiene los indices de los procesos que tienen su current_time - start_time % aging = 0 */
void queue_aging(Queue* from_queue, int current_time, Queue* to_queue);

/** Actualiza el estado de WAITING a READY de los procesos en la cola si completaron su tiempo de espera*/
void queue_update_waiting(Queue* queue);
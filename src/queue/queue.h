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

/** Funcion que destruye la cola liberando la memoria utilizada */
void queue_destroy(Queue* queue);

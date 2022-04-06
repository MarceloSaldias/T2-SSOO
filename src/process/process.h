#pragma once

enum Status {
  RUNNING,
  READY,
  WAITING,
  FINISHED
};

/** Estructura de un proceso **/
typedef struct process {
    // Nombre del proceso
    char* name;
    // Process pid
    int pid;
    // Tiempo de llegada a la cola
    int start_time;
    // Cantidad de tiempo que usará de la CPU
    int cycles;
    // Cantidad de ciclos que el proceso correrá antes de conceder
    // la CPU para esperar input de usuario
    int wait;
    // Cantidad de ciclos que el proceso esperará por el usuario
    // es decir, cantidad de tiempo que el proceso estará en WAITING
    int waiting_delay;
    // Cantidad de tiempo que debe pasar para que el proceso vuelva
    // a la cola de mayor prioridad
    int aging;
    // Prioridad
    int priority;
    enum Status status;
} Process;

///////////////////////////// Funciones publicas ///////////////////////////

/* Constructor de un proceso */
Process* process_init(int pid, char name[33], int priority, enum Status status, int aging);

/** Constructor de un proceso con array */
Process* process_init_array(char** args);

/** Funcion que destruye el proceso la memoria utilizada */
void process_destroy(Process* process);

#pragma once

enum Status {
  RUNNING,
  READY,
  WAITING,
  FINISHED
};

/** Estructura de un proceso **/
typedef struct process {
    // Process pid
    int pid;
    // Process name
    char name[33];
    // Process priority
    int priority;
    
    int start_time;
    int cycles;
    int wait;
    int waiting_delay;
    enum Status status;
    int aging;
} Process;

///////////////////////////// Funciones publicas ///////////////////////////

/** Constructor de un proceso */
Process* process_init(int pid, char name[33], int priority, enum Status status, int aging);

/** Funcion que destruye el proceso la memoria utilizada */
void process_destroy(Process* process);

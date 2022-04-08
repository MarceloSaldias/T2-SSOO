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
    // Prioridad del proceso (2 -> Mayor prioridad, 1 -> Prioridad Media, 0 -> Prioridad Baja)
    int priority;
    // Estado
    enum Status status;
    // ====== Los siguientes estados están para realizar analítica ======
    // Numero de veces que fue elegido por la CPU
    int times_chosen_by_cpu;
    // Numero de veces que fue interrumpido (Veces que el scheduler saco al proceso de la CPU)
    int times_interrupted;
    // Tiempo desde que entra hasta que termina su ejecucion
    int turnaround_time;
    // Tiempo desde que entra a la cola hasta su primera ejecucion
    int response_time;
    // Suma del tiempo en que el proceso esta en estado READY o WAITING
    int waiting_time;
} Process;

///////////////////////////// Funciones publicas ///////////////////////////

/* Constructor de un proceso */
Process* process_init(int pid, char name[33], int priority, enum Status status, int aging);

/** Constructor de un proceso con array */
Process* process_init_array(char** args);

/** Funcion que destruye el proceso la memoria utilizada */
void process_destroy(Process* process);

/** Funcion para imprimir un proceso */
void process_print(Process* process);

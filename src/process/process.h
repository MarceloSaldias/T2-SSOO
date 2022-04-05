#pragma once

enum Status {
  RUNNING,
  READY,
  WAITING,
  FINISHED
}

/** Estructura de un proceso **/
typedef struct process {
    // Process pid
    int pid;
    // Process name
    char[32] name;
    // Process priority
    int priority;
    
    int start_time;
    int cycles;
    int wait;
    int waiting_delay;
    Status status;
    int aging;
} Process;

// TODO: Faltan todas las fucnciones publicas
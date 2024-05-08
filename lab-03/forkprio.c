#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>      // times()
#include <unistd.h>
#include <signal.h>
#include <sys/resource.h>

int busywork(void)
{
    struct tms buf;
    for (;;) {
        times(&buf);
    }
}

void myHandlerKill() { 
    printf("\n");
    kill(0, SIGTERM);
}

void myHandler() {
    struct rusage usage;
    struct timeval system_time;

    getrusage(RUSAGE_SELF, &usage);
    system_time = usage.ru_stime;

    printf("Child %d (nice %2d):\t%3li\n", getpid(), getpriority(PRIO_PROCESS, 0), system_time.tv_sec);

    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
        printf("Uso: %s <número de hijos> <cantidad de segundos> <si se reduce la prioridad>\n", argv[0]);
        return 1;
    }

    int nProcesos = atoi(argv[1]);
    unsigned int nSegundos = (unsigned int) atoi(argv[2]);
    int reducirPrioridad = atoi(argv[3]);

    if (nProcesos <= 0) {
        printf("Número inválido de procesos.\n");
        return 1;
    }

     if (reducirPrioridad != 0 && reducirPrioridad != 1) {
        printf("Número inválido de reducción de prioridad, debe ser 1 o 0.\n");
        return 1;
    }

    int i;
    int pid;

    struct sigaction sa;
    sa.sa_handler = myHandler;
    sigaction(SIGTERM, &sa, NULL);

    if (nSegundos == 0) {
       sa.sa_handler = myHandlerKill;
       sigaction(SIGINT, &sa, NULL);
    }


    int pids[nProcesos];
    if (reducirPrioridad == 1) {

      for(i = 0; i < nProcesos; i++) {
         pid = fork();
         pids[i] = pid;
         if (pid == 0) {
            setpriority(PRIO_PROCESS, 0, i);
            busywork();
         } 
      }

    } else {
     
       for(i = 0; i < nProcesos; i++) {
          pid = fork();
          pids[i] = pid;
          if (pid == 0) 
            busywork();
       }

    }

    if (pid > 0)
       sleep(nSegundos);

    if (nSegundos == 0)
       pause();
    
    for(i = 0; i < nProcesos; i++) 
       kill(pids[i], SIGTERM);

    exit(EXIT_SUCCESS);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void myHandler(int sig) {
    printf("Señal recibida: %d %s\n", sig, strsignal(sig));
}

int main(void) {

    struct sigaction sa;
    sa.sa_handler = myHandler;

    while(1){
      int i;
      for (i = 1; i <= 64; i++) {
         sigaction(i, &sa, NULL);
      }

      printf("Número de proceso: %d\n", getpid());
      pause();
    }
    
    exit(EXIT_SUCCESS);
}

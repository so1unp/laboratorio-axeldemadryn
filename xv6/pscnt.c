#include "types.h"
#include "user.h"
#include "date.h"

int main(int argc, char *argv[])
{
    int pid = fork();

    if (pid1 == 0) {
        printf("Soy el primer proceso hijo, PID: %d\n", getpid());
    } else {
        pid2 = fork();
        
        if (pid2 == 0) {
            printf("Soy el segundo proceso hijo, PID: %d\n", getpid());
        } else {
            printf("Soy el proceso padre, PID: %d\n", getpid());
        }
    }

    printf(1, "%d\n", pscnt());
    exit();
}
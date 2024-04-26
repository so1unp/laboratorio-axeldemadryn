#include "types.h"
#include "user.h"
#include "date.h"

int busywork(void)
{
    for (;;) {
    }
}

int main(int argc, char *argv[])
{
    int i, pid;

    for (i = 0; i < 10; i++) {
       pid = fork();
       if (pid == 0) {
          busywork();
       }
    }

    printf(1, "%d\n", pscnt());
    exit();
}
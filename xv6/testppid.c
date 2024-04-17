/*
 * Invoca la llamada al sistema getppid().
 */
#include "types.h"
#include "user.h"
#include "date.h"

int main(int argc, char *argv[])
{
    printf(1, "%d\n", getppid());
    exit();
}
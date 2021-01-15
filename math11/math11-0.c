#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

const int MAX_COMMAND = 4096;

int main(int argc, char* argv[])
{
    char expr[MAX_COMMAND], command[MAX_COMMAND];
    fgets(expr, MAX_COMMAND, stdin);
    strtok(expr, "\n");
    sprintf(command, "print(%s)", expr);
    execlp("python3", "python3", "-c", command, NULL);
    perror("EXECUTION ERROR");
}
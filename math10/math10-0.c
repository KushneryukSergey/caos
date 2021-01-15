#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    for (int i = 0; 1; ++i) {
        int pid = fork();
        if (pid < 0) {
            printf("%d\n", i + 1);
            break;
        }
        if (pid) {
            int status;
            waitpid(pid, &status, 0);
            break;
        }
    }
    return 0;
}
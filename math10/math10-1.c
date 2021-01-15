#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int N = atoi(argv[1]);
    
    for (int i = 0; i < N; ++i) {
        int pid = fork();
        if (pid) {
            int status;
            waitpid(pid, &status, 0);
            if (i)
                printf("%d ", N - i);
            else {
                printf("%d\n", N);
            }
            fflush(stdout);
            break;
        }
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    setvbuf(stdin, NULL, _IONBF, 0);
    char buf[4097];
    pid_t pid;
    int result = 0;
    while (1) {
        pid = fork();
        if (pid == 0) {
            return (scanf("%s", buf) != EOF);
        } else {
            int status;
            waitpid(pid, &status, 0);
            if (!WEXITSTATUS(status)) {
                break;
            } else
                result += 1;
        }
    }
    printf("%d\n", result);
    return 0;
}
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

const int MAX_COMMAND = 4096;
const char tmp_program[] = "#include <stdio.h>\n"
                           "int main() { "
                           "printf(\"%%d\\n\", (%s)); "
                           "return 0; "
                           "}";

int main(int argc, char* argv[])
{
    char expr[MAX_COMMAND], command[MAX_COMMAND];
    fgets(expr, MAX_COMMAND, stdin);
    strtok(expr, "\n");
    sprintf(command, tmp_program, expr);
    char code[] = "main.c";
    char bin[] = "main";
    int flags = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
    int fd = open(code, O_WRONLY | O_CREAT, flags);
    dprintf(fd, "%s", command);
    // compile
    pid_t pid = fork();
    if (!pid) {
        execlp("gcc", "gcc", code, "-o", bin, NULL);
        perror("COMPILATION ERROR");
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
    // execute
    pid = fork();
    if (!pid) {
        execl(bin, bin, NULL);
        perror("EXECUTION ERROR");
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
    remove(code);
    remove(bin);
    return 0;
}
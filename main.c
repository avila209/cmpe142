//
// Created by Luis on 3/11/2019.
//

#include <err.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char **parser(char *line);
int executeCD(char *path);

int main()
{
    char *line = NULL;
    char **sep;
    size_t linesize = 0;
    ssize_t linelen;
    pid_t pid;
    int stat_loc;

    char home[1000];
    getcwd(home, 1000);

    while ((linelen = getline(&line, &linesize, stdin) != -1)) {
        printf("yeesh>");

        if(strncmp("exit", line, 4) == 0) {
            exit(0);
        }

        if(strncmp("cd", line, 2) == 0) {
            if(executeCD(sep[1]) < 0) {
                perror(sep[1]);
            }
            continue;
        }

        if(strncmp("path", line, 4) == 0) {
            //Do something;
        }

        sep = parser(line);
        pid = fork();
        if (pid == 0) {
            execvp(sep[0], sep);
            printf("Execution failed \n");
            break;
        }
        else {
            waitpid(pid, &stat_loc, WUNTRACED);
        }

    }

    free(line);
    if (ferror(stdin))
        err(1, "getline");
}


char **parser(char *line) {
    line = strtok(line, "\n");
    char *parsed;
    char **sep = malloc(8*sizeof(char *));
    int index = 0;
    printf("%s \n", line); //prints initial input

    parsed = strtok(line, " ");
    while(parsed != NULL){
        sep[index] = parsed;
        index++;
        parsed = strtok(NULL, " ");
    }

    sep[index] = NULL;

    return sep;
}


int executeCD(char *path) {
    printf("\n Path entered = %s ### \n", path);
    if(path == NULL){
        printf("Error my dude");
    }
    else {
        return chdir(path);
    }
}

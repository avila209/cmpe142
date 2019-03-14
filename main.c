//
// Created by Luis on 3/11/2019.
//

#include <err.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char *path = "/bin/";


char **parser(char *line);
int executeCD(char *direc);

int main()
{
    char *line = NULL;
    char **sep;
    size_t linesize = 0;
    ssize_t linelen;
    pid_t pid;
    int stat_loc;
    char *path = NULL;


    while (1) {
        printf("Thanos>");
        if((linelen = getline(&line, &linesize, stdin) == -1)){
            break;
        }
        sep = parser(line);


        if(strncmp("exit", line, 4) == 0) {
            exit(0);
        }

        if(strncmp("cd", line, 2) == 0) {
            if(sep[2] != NULL){
                printf("Error: more than one argument passed. \n");
            }
            if(executeCD(sep[1]) < 0) {
                printf("Error: directory change failed, bad address. \n");
            }
            continue;
        }

        if(strncmp("path", line, 4) == 0) {
            //path = sep[1]; Need to be able to take multiple arugments and store into char** not char*.
        }

        else {
            pid = fork();
            if (pid == 0) {
                execv(sep[0], sep);
                printf("Execution failed \n");
                break;
            } else {
                waitpid(pid, &stat_loc, WUNTRACED);
            }
        }

    }

    free(line);
    if (ferror(stdin))
        err(1, "getline");
}


char **parser(char *line) {
    line = strtok(line, "\n");

    char *tmp = malloc(8*sizeof(line));
    strcat(tmp, path);
    strcat(tmp, line);


    char *parsed;
    char **sep = malloc(8*sizeof(char *));
    int index = 0;

    parsed = strtok(tmp, " ");
    while(parsed != NULL){
        sep[index] = parsed;
        index++;
        parsed = strtok(NULL, " ");
    }

    sep[index] = NULL;

    return sep;
}


int executeCD(char *direc) {
    if(direc == NULL){
        printf("Error: No directory passed. \n");
    }
    else {
        return chdir(direc);
    }
}

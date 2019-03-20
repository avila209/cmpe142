//
// Created by Luis on 3/11/2019.
//

#include <err.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char **path;
char *currentDir;


char **parser(char *line);
int executeCD(char *direc);
char **setPath(char **sep);
void **InitializePath(char **sep);

int main()
{
    char buff[1000];
    currentDir = getcwd(buff,sizeof(buff));

    char *line = NULL;
    char **sep;
    size_t linesize = 0;
    ssize_t linelen;
    pid_t pid;
    int stat_loc;

    //setting default path
    path = malloc(8*sizeof(char*));
    char *defaultPath = malloc(sizeof(5));
    strcpy(defaultPath, "/bin/");
    path[0] = defaultPath;



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

        if(strncmp("path", line, 4) == 0){
            path = setPath(sep);
            printf("Path set to %s \n", *path);
        }

        else {
            InitializePath(sep);

            pid = fork();
            int i = 0;
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

    char *parsed;
    char **sep = malloc(8*sizeof(char *));
    int index = 0;

    parsed = strtok(line, " ");
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
        return chdir(currentDir);
    }

    else {
        return chdir(direc);
    }
}

char **setPath(char **sep){
    char **tmpPATH;
    tmpPATH = ++sep;
    return tmpPATH;
}

void **InitializePath(char **sep){
    char *tmp = malloc(8*sizeof(char*));
    strcpy(tmp, path[0]);
    strcat(tmp, sep[0]);
    sep[0] = tmp;
}

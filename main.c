//
// Created by Luis on 3/11/2019.
//

#include <err.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

char *currentDir;
char error_message[30] = "An error has occurred\n";

char** parser(char *line);
int executeCD(char *direc);
char** setPath(char *line);

int redirection(char **sep, char **output_filename);
int parallel_commands(char **sep);
char **initializepath(char **sep, char **path, int i);

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
    char **path = malloc(8*sizeof(char*));
    char *defaultPath = malloc(sizeof(char*));
    strcpy(defaultPath, "/bin");
    path[0] = defaultPath;

    //Redirection vars
    int output = 0;
    char **output_filename = malloc(2*sizeof(char*));

    //Parallel command vars
    int parallel;

    char **command = malloc(8*sizeof(32));

    while (1) {
        /*
        strcpy(defaultPath, "/bin/");
        path[0] = defaultPath;
        */

        printf("Thanos>");
        if((linelen = getline(&line, &linesize, stdin) == -1)){
            break;
        }

        else if(strncmp("exit", line, 4) == 0) {
            exit(0);
        }

        else if(strncmp("cd", line, 2) == 0) {
            sep = parser(line);
            if(sep[2] != NULL){
                printf("%s \n", error_message);
            }
            if(executeCD(sep[1]) < 0) {
                printf("%s \n", error_message);
            }
            continue;
        }

        else if(strncmp("path", line, 4) == 0){
            path = setPath(line);
        }

        else {
            sep = parser(line);
            output = redirection(sep, output_filename);
            parallel = (parallel_commands(sep) == 0);


            pid = fork();
            if (pid == 0) {

                if(output){
                        freopen(output_filename[0], "w", stdout);
                        freopen(output_filename[0], "w", stderr);
                }
                int i = 0;
                while(path[i]){
                    command = initializepath(sep, path, i);
                    if(access(command[0], X_OK) == 0){
                        break;
                    }
                    else{
                        i++;
                    }
                }
                execv(command[0], command);
                printf("%s \n", error_message);
                break;
            }
            else{
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
    char *delim = " \t\r\n\v\f";

    parsed = strtok(line, delim);
    while(parsed != NULL){
        sep[index] = parsed;
        index++;
        parsed = strtok(NULL, delim);
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

char **setPath(char *line) {
    line = strtok(line, "\n");

    char *parsed;
    char **path = malloc(8*sizeof(char *));
    int index = 0;

    parsed = strtok(line, " ");
    parsed = strtok(NULL, " ");
    while(parsed != NULL){
        path[index] = parsed;
        index++;
        parsed = strtok(NULL, " ");
    }

    path[index] = NULL;
    return path;
}


int redirection(char **sep, char **output_filename){
    int i, j;

    for(i=0; sep[i] != NULL; i++){
        if(sep[i][0] == '>') {
            sep[i] = NULL;

            if(sep[i+1] != NULL && sep[i+2] == NULL) {
                output_filename[0] = sep[i + 1];
            }
            else if(sep[i+2] != NULL){
                printf("%s \n", error_message);
            }
            else{
                printf("%s \n", error_message);
                return 0;
            }

            for(j=1; sep[j-1] != NULL; j++){
                sep[j] = sep[j+2];
            }
            return 1;
        }
    }
    return 0;
}

int parallel_commands(char **sep) {
    int i;
    for (i = 1; sep[i] != NULL; i++) {
        if (sep[i-1][0] == '&') {
            free(sep[i-1]);
            sep[i-1] = NULL;
            printf("found an & \n");
            return 1;
        }
    }
    return 0;
}

char **initializepath(char **sep, char **path, int i){
    int j = 1;
    char **command = malloc(8*sizeof(32));
    char *slash = malloc(sizeof(1));
    strcpy(slash, "/");
    command[0] = strdup(path[i]);
    strcat(command[0], slash);
    strcat(command[0], sep[0]);
    while(sep[j] != NULL){
        command[j] = sep[j];
        j++;
    }
    command[j] = NULL;
    return command;
}
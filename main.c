//
// Created by Luis on 3/11/2019.
//

#include <err.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

char **parser(char *line);

int
main(int argc, char **argv)
{
    char *line = NULL;
    char *cmd = NULL;
    char *argument = NULL;
    char **sep;
    size_t linesize = 0;
    ssize_t linelen;
    pid_t pid;
    int stat_loc;

    while ((linelen = getline(&line, &linesize, stdin)) != -1) {
        printf("yeesh>");
        if (strncmp("exit", line, 4) == 0) {
            // user wants to exit
            exit(0);
        }
        cmd = sep[0];
        argument = sep[1];
        sep = parser(line);

        pid = fork();
            if(pid == 0) {
                execvp(sep[0], sep);
                printf("... yo this shouldnt print man, shit broke \n");
            }
            else{
                waitpid(pid, &stat_loc, WUNTRACED);
            }
    }

    free(line);
    if (ferror(stdin))
        err(1, "getline");
}


char **parser(char *line) {
    char *parsed;
    char **sep = malloc(8*sizeof(char *));
    int index = 0;
    printf("%s", line); //prints initial input

    parsed = strtok(line, " ");
    while(parsed != NULL){
        sep[index] = parsed;
        index++;
        parsed = strtok(NULL, " ");
    }

    sep[index] = NULL;
    return sep;
}

//
// Created by Luis on 3/11/2019.
//

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
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

    while ((linelen = getline(&line, &linesize, stdin)) != -1) {
        printf("yeesh>");
        if (strncmp("exit", line, 4) == 0) {
            // user wants to exit
            exit(0);
        }

        sep = parser(line);

        cmd = sep[0];
        argument = sep[1];

        printf("Command line = %s \n", cmd);
        printf("Argument line = %s \n", argument);

        // fork
        if(strncmp("fork", line, 4) == 0) {
            pid = fork();
            wait(1);
            if(pid == 0) {
                printf("The DNA says, I am your child");
            }
            else if(pid > 0) {
                printf("The DNA says, you are the father");
            }
            else {
                printf("...somethin aint right bruh");
            }
        }
        // if child, exec
        // if parent, wait

        // ls
        if(strncmp("ls", line, 2) == 0) {
            printf("ok ok we gon do dis");
        }

        if(strncmp("./", line, 2) == 0) {
            printf("put em in the coffin");
        }
    }

    free(line);
    if (ferror(stdin))
        err(1, "getline");
}


char **parser(char *line) {
    // Returns first token
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "shell.h"



int main(void){
    //Estado para el hijo.
    int estado;
    do{
        //para ingresar el comando se almacenara en la siguiente variable
        char comando[CARACTER_MAX_COMAND];
        shell();
        scanf(" %255[^\n]",comando);

        //si escribimos en la terminal el comando exit podremos salir de la shell
        //ya sea usando el comando "exit" o "quit"
        if (!strcmp("exit",comando)){
            printf("Saliendo del Shell...\n");
            break;
            exit(EXIT_SUCCESS);
        }
        if (!strcmp("quit",comando)){
            printf("Saliendo del Shell...\n");
            break;
            exit(EXIT_SUCCESS);
        }

        pid_t pid = fork();
        assert(pid >= 0);

        //Para la creacion del proceso hijo
        if (!pid){
            readCommand(comando);
        }
        //verificacion del estado padre
        else if (pid > 0){
            wait(&estado);
            //si es que esta de salida
            if (WIFEXITED(estado)){
                if (!(WEXITSTATUS(estado))){
                    //limpear la consola
                    if(!strcmp("clear",comando)){
                        printf("");
                    } else{
                        printf("\n\n"); //saltos de lina despues de un comando
                    }
                } else{
                    printf("\n:(\n");
                }
            } else{
                return 1;
            }
        //anti error al momento de intentar crear el pid
        } else{
            perror("|ERROR| creacion de PID\n");
            exit(EXIT_FAILURE);
        }
    } while (1);
}

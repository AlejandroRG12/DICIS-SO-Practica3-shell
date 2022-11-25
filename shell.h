#ifndef __shell_h__
#define __shell_h__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <sys/wait.h>
#include <linux/limits.h>

#define COLOR_VERDE "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define CARACTER_MAX_COMAND 256
#define MAXSTR 256
#define READ  0
#define WRITE 1


//ejecucion del promp del shell
void shell();

//Esta funcion lee el comando ingresado y en base a eso determinar si este esta definido
void readCommand(char* comando);

//cuando el comando contiene espacios
void commandSpace(char* comando);

//desglosa un comando pipe y los almacena en un arreglo de punteros
void pipePunterCommand(char* comandoPipe);

// desglosa un comando > y los granda en un arreglo de punteros
void pOComando(char* comandoOut);

//Esta funcion esta destinada para comandos basicos
void basicCommand(int argc, char* argv[]);

//funcion para el uso de un comando con pipe |
void pipeCommand(char* pComando, char* sComando);

//para cuando un archivo contiene uns alida >
void outFileCommand(char* comando, char* salida);

//funcion para cuando un cadena de texto contenga espacios en blanco estos sean eliminados
void spacesRemove(char* str);

#endif

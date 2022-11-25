#include "shell.h"

void shell(){
    char hostname[PATH_MAX+1];
    char login[PATH_MAX+1];
    char directory[PATH_MAX+1];
    char flag[] = "/";

    //sustitucion de infomacion
    gethostname(hostname, sizeof(hostname));
    getlogin_r(login, sizeof(login));
    getcwd(directory, sizeof(directory));

    char *dir = strtok(directory, flag);

    if(dir != NULL){
        while(dir != NULL){
            strcpy(directory, dir);
            dir = strtok(NULL, flag);
        }
    }

    printf(COLOR_VERDE "[%s@%s " ANSI_COLOR_RESET ,login,hostname);
    printf("%s]$ ",directory);
}

void commandSpace(char* comando){
    char* cadena[CARACTER_MAX_COMAND];
    const char delimitador[] = " "; //para delimitar
    int i = 0;
    char *token = strtok(comando, delimitador);

    if(token != NULL){
        do{
            cadena[i] = token;
            i++;
            token = strtok(NULL, delimitador);
        }while(token != NULL);
        cadena[i+1] = "\0";
    }

    commandBasic(i,cadena);
}


void pPComando(char* comandoPipe){
    char p1Cadena[CARACTER_MAX_COMAND];
    char p2Cadena[CARACTER_MAX_COMAND];
    const char delimitador[] = "|";

    //para el ingreso de comandos
    char *token = strtok(comandoPipe, delimitador);
    strcpy(p1Cadena,token);
    token = strtok(NULL, delimitador);
    strcpy(p2Cadena,token);
	
    commandPipe(p1Cadena,p2Cadena);
}

void pOComando(char* comandoOut) {
    char p1Cadena[CARACTER_MAX_COMAND];
    char p2Cadena[CARACTER_MAX_COMAND];
    const char delimitador[] = ">";

    //insertar comado
    char *token = strtok(comandoOut, delimitador);
    strcpy(p1Cadena,token);
    token = strtok(NULL, delimitador);
    strcpy(p2Cadena,token);

    commandOutFile(p1Cadena, p2Cadena);
}

void readCommand(char* comando){
    int pipe = 124, salida = 62;

    /* Si hay pipe. */
    if (strchr(comando, pipe)) {
        pPComando(comando);
    } else if (strchr(comando, salida)) { //Si hay salida.
        pOComando(comando);
	} else {  /* Comando basico. */
        commandSpace(comando);
    }    
}


void commandBasic(int argc, char* argv[]){
    argv[argc] = NULL;

    execvp(argv[0], argv);

    fprintf(stderr,":( %s: no esta definido.\n", argv[1]);
    exit(EXIT_FAILURE);
}


void commandPipe(char* pComando, char* sComando){
    pid_t pid;
    int fd[2];

    if (pipe(fd) == -1) {
	    perror("Creating pipe");
	    exit(EXIT_FAILURE);
    }

    switch(pid = fork()) {

        case 0:
	        // El proceso hijo se ejecuta con sComando.
	        // Cierra la escritura del descriptor del pipe.
	        close(fd[WRITE]);
	        // Redirect STDIN to read from the pipe.
	        dup2(fd[READ], STDIN_FILENO);
	        // Ejecuta segundo comando.
	        readCommand(sComando);

        case -1:
	        perror("fork() failed)");
    	    exit(EXIT_FAILURE);

        default:
	        // El proceso padre se manda a pComando.
	        // Se cierra el pipe que lee el descriptor.
	        close(fd[READ]);
	        // Redirecciona STDOUT a la escritura del pipe.
	        dup2(fd[WRITE], STDOUT_FILENO);
	        // Ejecuta primer comando.
	        readCommand(pComando);
    }
}

void commandOutFile(char* comando, char* salida){

    char cadena[MAXSTR];
	int s;
	int file; /* descriptor del archivo que se va a usar. */
	
	bzero(cadena, sizeof(char)*MAXSTR);
	remove_spaces(salida);
	file = open(salida, O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
	
	assert(file != -1);

	dup2(file, STDOUT_FILENO);
	commandSpace(comando);
	close(file);
}


void remove_spaces(char* str) {
    char *write = str, *read = str;
	
	do {
	   if (*read != ' ')
    	   *write++ = *read;
	} while (*read++);
}

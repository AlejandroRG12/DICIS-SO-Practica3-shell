#include "shell.h"

void shell(){
    char name[PATH_MAX+1];
    char directory[PATH_MAX+1];
    char login[PATH_MAX+1];
    char flag[] = "/";

    //sustitucion de infomacion
    gethostname(name, sizeof(name));
    getlogin_r(login, sizeof(login));
    getcwd(directory, sizeof(directory));

    char *dir = strtok(directory, flag);

    if(dir != NULL){
        while(dir != NULL){
            strcpy(directory, dir);
            dir = strtok(NULL, flag);
        }
    }

    printf(COLOR_VERDE "[%s@%s " ANSI_COLOR_RESET ,login,name);
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

    basicCommand(i,cadena);
}


void pipePunterCommand(char* comandoPipe){
    char cadenaN1[CARACTER_MAX_COMAND];
    char cadenaN2[CARACTER_MAX_COMAND];
    const char delimitador[] = "|";

    //para el ingreso de comandos
    char *token = strtok(comandoPipe, delimitador);
    strcpy(cadenaN1,token);
    token = strtok(NULL, delimitador);
    strcpy(cadenaN2,token);
	
    pipeCommand(cadenaN1,cadenaN2);
}

void pOComando(char* comandoOut) {
    char cadenaN1[CARACTER_MAX_COMAND];
    char cadenaN2[CARACTER_MAX_COMAND];
    const char delimitador[] = ">";

    //insertar comado
    char *token = strtok(comandoOut, delimitador);
    strcpy(cadenaN1,token);
    token = strtok(NULL, delimitador);
    strcpy(cadenaN2,token);

    outFileCommand(cadenaN1, cadenaN2);
}

void readCommand(char* comando){
    int pipe = 124;
    int salida = 62;

    /* Si hay pipe. */
    if (strchr(comando, pipe)) {
        pipePunterCommand(comando);
    } else if (strchr(comando, salida)) { //Si hay salida.
        pOComando(comando);
	} else {  
        commandSpace(comando);  //comando basico
    }    
}


void basicCommand(int argc, char* argv[]){
    argv[argc] = NULL;

    execvp(argv[0], argv);

    fprintf(stderr,"%sERROR: Comando no establecido\n", argv[1]);
    exit(EXIT_FAILURE);
}


void pipeCommand(char* pComando, char* sComando){
    pid_t pid;
    int fd[2];

    if (pipe(fd) == -1) {
	    perror("Creando |");
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
	        perror("Error");
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

void outFileCommand(char* comando, char* salida){

    char cadena[MAXSTR];
	int s;
	int file; /* descriptor del archivo que se va a usar. */
	
	bzero(cadena, sizeof(char)*MAXSTR);
	spacesRemove(salida);
	file = open(salida, O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
	
	assert(file != -1);

	dup2(file, STDOUT_FILENO);
	commandSpace(comando);
	close(file);
}


void spacesRemove(char* str) {
    char *write = str, *read = str;
	
	do {
	   if (*read != ' ')
    	   *write++ = *read;
	} while (*read++);
}

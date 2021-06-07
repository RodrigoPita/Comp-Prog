#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <string.h>

void read_command (char cmd[], char *par[]) {
	char line[1024];
	int count = 0, i = 0, j = 0;
	char *array[100], *pch;

	// le uma linha
	for ( ;; ) {
		int c = fgetc (stdin);
		line[count++] = (char) c;
		if (c == '\n') break;
	}
	if (count == 1) return;
	pch = strtok (line, " \n");

	// pega as palavras da linha
	while (pch != NULL) {
		array[i++] = strdup (pch);
		pch = strtok (NULL, " \n");
	}
	// primeira palavra eh o comando
	strcpy (cmd, array[0]);

	// as outras, parametros
	for(int j = 0; j < i; j++)
		par[j] = array[j];
	par[i] = NULL;	// NULL-acaba com a lista de parametros
}

void type_prompt() {
	static int first_time = 1;
	if (first_time) {	//limpa a tela pela primeira vez
		const char* CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
		write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
		first_time = 0;
	}

	printf("$$");
}

int main() {
	char cmd[100], command[100], *parameters[20];
	// variavel do ambiente
	char *envp[] = { (char *) "PATH=/bin", 0};
	while (1) {				//loop infinito
		type_prompt();		//Mostra o prompt na tela
		read_command (command, parameters);	// le a entrada do terminal
		if (fork() != 0)	//processo pai
			wait (NULL);	//espera o processo filho
		else {
		strcpy (cmd, "/bin/");
		strcat (cmd, command);
		execve (cmd, parameters, envp);		// executa o comando
		}
		if (strcmp (command, "exit") == 0)
			break;
	}

	return 0;
}

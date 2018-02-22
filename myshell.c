#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFSIZE 512

void parseCommands(char *input, char **commands, char *delimiter, int *commandsCount);
void parseArgs(char *command, char **args);
int verifyDelims(char *input, char *delimiter);
char findDelim(char *input);


int main(int argc, char **argv) {
	pid_t pid, wpid;
	int batchMode = 0;
	FILE *fp;
	char delimiter;
	int commandsCount;

	if (argc > 1) {
		batchMode = 1;
		fp = fopen(argv[1], "r");
		if (fp == NULL) {
			printf("Error accessing batch file!\n");
			exit(1);
		}
	}

	//Main shell loop
	while(1){

		char input[BUFSIZE];
		char *args[BUFSIZE];
		char *commands[BUFSIZE];

		if(batchMode) {
			if(fgets(input, BUFSIZE, fp) == NULL) {
				fclose(fp);
				exit(0);
			}
		}

		else {

			printf("galazios_8614> ");
			fgets(input, BUFSIZE, stdin);

		}

		//Verify and parse command
		delimiter = findDelim(input);

		if (!verifyDelims(input, &delimiter) ) {
			printf ("Valid delimiters are: && and ;\nExiting.\n");
			exit(0);
		}

		parseCommands(input, commands, &delimiter, &commandsCount);
		if(delimiter == ' ') {
			commandsCount = 1;
		}

		int i = 0;

		while (i < commandsCount) {

			//Fork
			if ((pid = fork()) < 0) {
				printf("Fork failed!");
				exit(1);
			}

			else if (pid == 0) {

				parseArgs(commands[i], args);

				if(!strcmp(args[0], "quit")) {
					_exit(7);
				}

				int error_status;
				error_status = execvp(args[0], args);
				printf("Failed executing %s command.\n", args[0]);
				_exit(2);

			}

			else {

				//Wait for child processes
				int status = 0;
				while ( (wpid = wait(&status) ) > 0);

				if ( WIFEXITED(status) ) {
					int exitcode = WEXITSTATUS(status);
					if (exitcode == 7) exit(0);
					if (exitcode == 2 && delimiter == '&') break;
				}

				i++;

			}
		}
	}
}

void parseCommands(char *input, char **commands, char *delimiter, int *commandsCount) {

	input = strtok(input, "\r\n");

	if (*delimiter == ' ') {
		
		//Only 1 command
		commands[0] = input;
		*commandsCount = 1;
		
	}
	else {

		char *token;

		//Break each command into arguments
		token = strtok(input, delimiter);

		int i = 0;
		while(token != NULL) {
			commands[i] = token;
			token = strtok(NULL, delimiter);
			i++;
		}

		*commandsCount = i;
	}
}

void parseArgs(char *command, char **args) {
	char *token;

	token = strtok(command, " ");

	int i = 0;
	while(token != NULL) {
		args[i] = token;
		token = strtok(NULL, " ");
		i++;
	}
	//execvp requires NULL as last argument
	args[i] = NULL;
}

int verifyDelims(char *input, char *delimiter) {
	int check = 1;
	char *ret;
	char *stack = input;

	if (*delimiter == '&') {

		//Check if there are exactly 2 &s
		//for every & occurence
		while( (ret = strstr(stack, "&")) != NULL ){

			if(ret[1] != '&' || ret[2] == '&'){
				check = 0;
				break;
			}

			stack = ret + 2;
		}
	}
	
	return check;
}

char findDelim(char *input) {
	char *ret;

	ret = strstr(input, "&");
	if (ret != NULL) return '&';

	ret = strstr(input, ";");
	if (ret != NULL) return ';';

	//Using space to indicate no delimiter
	//(only 1 command)
	return ' ';
}

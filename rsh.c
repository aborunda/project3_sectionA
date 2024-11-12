#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise	

	for (int i = 0; i < N; i++) {
		if (strcmp(cmd,allowed[i])==0) {
			return 1;
		}
	}
	
	return 0;
}

// Function to print the help message
void help() {
	printf("The allowed commands are:\n");
	for (int i = 0; i < N; i++) {
		printf("%d: %s\n", i + 1, allowed[i]);
	}
}


int main() {

    // TODO
    // Add variables as needed

	char line[256];

    while (1) {
		fprintf(stderr,"rsh>");

		if (fgets(line,256,stdin)==NULL) continue;
		if (strcmp(line,"\n")==0) continue;

		line[strlen(line)-1]='\0';

		/*
		TODO
		Add code to spawn processes for the first 9 commands
		And add code to execute cd, exit, help commands
		Use the example provided in myspawn.c
		*/
		
		/*
		Tokenize the input line into command and arguments
		*/
		// argv[21] is used to store the arguments
		char *argv[21];
		// argc is used to store the number of arguments
		int argc = 0;
		// Tokenize the input line into command and arguments
		char *token = strtok(line, " ");
		// Iterate over the tokens and store them in argv
		while(token != NULL && argc < 20) {
			argv[argc++] = token;
			token = strtok(NULL, " ");
		}
		// Set the last element of argv to NULL
		argv[argc] = NULL;
		// If the command is exit, break the loop
		if (argc == 0) continue;

		/*
		Check if the command is allowed
		*/
		if (!isAllowed(argv[0])) {
			printf("NOT ALLOWED!\n");
			continue;
		}

		/*
		Handle the built-in commands
		*/
		if (strcmp(argv[0], "cd") == 0) {
			// Change the directory
			if (argc != 2) {
				printf("-rsh: cd: too many arguments\n");
			} else {
				if (chdir(argv[1]) != 0) {
					perror("cd");
				}
			}
		} else if (strcmp(argv[0], "exit") == 0) {
			// Exit the shell
			return 0;
		} else if (strcmp(argv[0], "help") == 0) {
			// print the help message
			help();
		} else {
			// Spawn a new process for the first 9 commands
			pid_t pid;
			int status;
			if (posix_spawnp(&pid, argv[0], NULL, NULL, argv, environ) != 0) {
				perror("posix_spawnp");
			} else {
				waitpid(pid, &status, 0);
			}
		}
    }
    return 0;
}

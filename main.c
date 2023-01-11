#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include"header.h"

#define LINE_LEN 80
#define MAX_ARGS 64
#define MAX_ARG_LEN 16
#define MAX_PATHS 64
#define MAX_PATH_LEN 96
#define WHITESPACE " ,\t\n"

// main starts here
int main() {
	/* Shell initialization */

	char* completepath = NULL;
	char command[100];
	char* parameters[100];

	/* Get directory paths from PATH */
	char* dirs[50];
	char* argv[MAX_ARG_LEN];

	for (int i = 0;i < MAX_ARG_LEN;i++) {
		argv[i] = NULL;
		argv[i] = (char*)malloc(100);
	}
	parsePath(dirs);
	while (1) {
		type_prompt(); //display prompt on screen

		/* Read the command line and parse it */
		read_command(command, parameters);

		if (strcmp(command, "exit") == 0) {
			break;
		}

		strcpy(argv[0], command);

		/* Get the full pathname for the file */
		completepath = lookupPath(argv, dirs);

		int id = fork();

		/* Report error */
		if (completepath == NULL) {
			printf("%s: Command not found\n", argv[0]);
		}
		else {
			//child process
			if (id == 0) {
				execv(completepath, parameters);
				exit(0);
			}

			//parent process
			else if (id > 0) {
				wait(NULL);
				printf("Parent Process called\n");

			}
		}
	}
	return 0;

}

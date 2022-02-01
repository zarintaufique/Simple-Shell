#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include"19L-1029_header.h"

#define LINE_LEN 80
#define MAX_ARGS 64
#define MAX_ARG_LEN 16
#define MAX_PATHS 64
#define MAX_PATH_LEN 96
#define WHITESPACE " ,\t\n"


static int first_time = 1;
static char user[100] = "zarintaufique@ubuntu";
static char currentpath[100] = "~";
const char* CLEAR_SCREEN = "\e[1;1H\e[2J";

void type_prompt() {
	if (first_time) { //clear screen for the first time
		write(STDOUT_FILENO, CLEAR_SCREEN, 12);
	}
	first_time = 0;
	printf("%s:%s$", user, currentpath);
}

void read_command(char cmd[], char* par[]) {
	char line[1024];
	int count = 0, i = 0, j = 0;
	char* array[100], * pch;

	//read one line
	for (; ; ) {
		int c = fgetc(stdin);
		line[count++] = (char)c;
		if (c == '\n') { //complete line has been entered
			line[count] = '\0';
			break;
		}
	}

	if (count == 1) {
		return;
	}

	//breaking into tokens
	pch = strtok(line, WHITESPACE);

	while (pch != NULL) {
		//putting in a variable array	
		array[i++] = strdup(pch);
		pch = strtok(NULL, WHITESPACE);
	}

	//first word is the command
	strcpy(cmd, array[0]);

	//other are parameters
	for (int j = 0;j < i;j++) {
		par[j] = array[j];
	}
	par[i] = NULL; //null terminate the parameter list	
}

int parsePath(char* dirs[]) {
	/* This function reads the PATH variable for this
	* environment, then builds an array, dirs[], of the
	* directories in PATH
	*/
	char* pathEnvVar;
	char* thePath;
	int j = 0;
	char* pch;

	for (int i = 0; i < MAX_PATHS; i++)
		dirs[i] = NULL;

	pathEnvVar = (char*)getenv("PATH");
	thePath = (char*)malloc(strlen(pathEnvVar) + 1);
	strcpy(thePath, pathEnvVar);

	/* Loop to parse thePath. Look for a ":"
	* delimiter between each path name.
	*/
	pch = strtok(thePath, ":");
	while (pch != NULL) {
		//putting in a variable array
		dirs[j++] = strdup(pch);
		pch = strtok(NULL, ":");
	}
	return j;
}

char* lookupPath(char* argv[], char* dir[]) {
	char* result = (char*)malloc(MAX_PATH_LEN);
	int found = 0;


	// Check to see if file name is already an absolute path
	if (*argv[0] == '/') {
		return argv[0];
	}
	else {

		for (int i = 0; dir[i] != NULL; i++) {
			strcpy(result, dir[i]);
			strcat(result, "/");
			strcat(result, argv[0]);
			//printf("Path found: %s", result);

			found = access(result, F_OK);

			//path found
			if (found == 0) {
				return result;
			}
		}

		// File name not found in any path variable
		return NULL;
	}
}

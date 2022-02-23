#ifndef HEADER_H
#define HEADER_H

void type_prompt();

void read_command(char cmd[], char* par[]);

int parsePath(char* dirs[]);


char* lookupPath(char* argv[], char* dir[]);

#endif

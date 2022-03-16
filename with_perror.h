#include <dirent.h>
#include <stdio.h>
const char* field_name = "COMMAND         PID             USER            FD              TYPE            NODE            NAME";

DIR* Opendir(const char *name){
	DIR *dp = opendir(name);
	if(dp == NULL) perror("Opendir");
	return dp;
}

FILE* Fopen(const char* pathname, const char* mode){
	FILE* file = fopen(pathname, mode);
	if(file == NULL) perror("fopen");
	return file;
}
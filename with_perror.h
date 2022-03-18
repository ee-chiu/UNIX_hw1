#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>

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

int Stat(const char* pathname, struct stat *statbuf){
	int s = stat(pathname, statbuf);
	if(s < 0) perror("stat");
	return s;
}
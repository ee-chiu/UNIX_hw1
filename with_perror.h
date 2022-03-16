#include <dirent.h>
#include <stdio.h>
const char* field_name = "COMMAND         PID             USER            FD              TYPE            NODE            NAME";

DIR* Opendir(const char *name){
	DIR* dp = opendir(name);
	if(dp == NULL) perror("Opendir");
	return dp;
}

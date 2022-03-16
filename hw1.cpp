#include "with_perror.h"
#include "without_perror.h"
#include <stdio.h>
#include <dirent.h>
#include <string.h>

char* get_command(const char *dir){
    char *cmdline_path = new char[30];
    strcpy(cmdline_path, dir);
    strcat(cmdline_path, "/cmdline");
    FILE *cmdline_file = fopen(cmdline_path, "r");
    char *cmdline_txt = new char[1000];
    fscanf(cmdline_file, "%s", cmdline_txt);
    return cmdline_txt;
}

void get_info(const char *dir){
    char* cmdline_txt = get_command(dir);
    printf("%s\n", cmdline_txt);
}


int main(int argc, char** argv){
    printf("%s\n", field_name);
	DIR *dp = Opendir("/proc");
    struct dirent *dirp;
    while((dirp = readdir(dp)) != NULL){
        if(isnum(dirp->d_name)) {
            char dir[20] = "/proc/";
            strcat(dir, dirp->d_name);
            get_info(dir);            
            }
    }
    return 0;
}

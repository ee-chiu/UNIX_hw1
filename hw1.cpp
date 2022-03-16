#include "with_perror.h"
#include "without_perror.h"
#include <stdio.h>
#include <dirent.h>
#include <string.h>

struct info{
    char* command;
    char* pid;
    char* user;
    char* fd;
    char* type;
    char* node;
    char* name;
};

char* get_command(const char *dir){
    char *cmdline_path = new char[30];
    strcpy(cmdline_path, dir);
    strcat(cmdline_path, "/status");
    FILE *cmdline_file = fopen(cmdline_path, "r");
    char *cmdline_txt = new char[1000];
    fscanf(cmdline_file, "Name: %[^\n]", cmdline_txt);
    return cmdline_txt;
}

info get_info(const char *dir, char *pid){
    info tmp;
    char* cmdline_txt = get_command(dir);
    tmp.command = cmdline_txt;
    tmp.pid = pid;
    return tmp;
}


int main(int argc, char** argv){
    printf("%s\n", field_name);
	DIR *dp = Opendir("/proc");
    struct dirent *dirp;
    while((dirp = readdir(dp)) != NULL){
        if(isnum(dirp->d_name)) {
            char dir[20] = "/proc/";
            strcat(dir, dirp->d_name);
            info tmp = get_info(dir, dirp->d_name);
            printf("%s\t\t%s\n", tmp.command, tmp.pid);            
            }
    }
    return 0;
}

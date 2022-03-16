#include "with_perror.h"
#include "without_perror.h"
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <vector>

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
    char* status_path = new char[30];
    strcpy(status_path, dir);
    strcat(status_path, "/status");
    FILE* status_file = fopen(status_path, "r");
    char* command = new char[1000];
    fscanf(status_file, "Name: %[^\n]", command);
    return command;
}

char* get_user(const char* dir){
    char* this_uid = get_uid(dir);
    FILE* passwd_file = Fopen("/etc/passwd", "r");
    char* user_info_line = new char[500];
    char* user = new char[100];
    char* uid = new char[10];
    for( ; ; ){
        fgets(user_info_line, 500, passwd_file);
        user = strtok(user_info_line, ":");
        strtok(NULL, ":");
        uid = strtok(NULL, ":"); 
        if(strcmp(this_uid, uid) == 0) {
            break;
        }
    }
    return user;  
}

info get_info(const char *dir, char *pid){
    info tmp;
    char* command = get_command(dir);
    tmp.command = command;
    tmp.pid = pid;
    tmp.user = get_user(dir);
    return tmp;
}


int main(int argc, char** argv){
    printf("%s\n", field_name);
	DIR* dp = Opendir("/proc");
    struct dirent* dirp;
    std::vector<info> info_list;

    while((dirp = readdir(dp)) != NULL){
        if(isnum(dirp->d_name)) {
            char dir[20] = "/proc/";
            strcat(dir, dirp->d_name);
            info tmp = get_info(dir, dirp->d_name);
            info_list.push_back(tmp);
            }
    }

    for(info tmp : info_list){
        printf("%s\t\t", tmp.command);            
        printf("%s\t\t", tmp.pid);
        printf("%s\t\t", tmp.user);
        printf("\n");
    }
    return 0;
}

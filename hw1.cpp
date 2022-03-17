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

std::vector<info> get_info(const char *dir, char *pid){
    std::vector<const char*> match_file_list = match_file(dir);
    char* command = get_command(dir);
    char* user = get_user(dir);
    std::vector<info> tmp_list;
    for(const char* file_name : match_file_list){
        info tmp;
        tmp.command = command;
        tmp.pid = pid;
        tmp.user = user;
        tmp.fd = fd_name(file_name);
        tmp_list.push_back(tmp);
    }
    return tmp_list;
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
            std::vector<info> tmp_list = get_info(dir, dirp->d_name);
            for(info tmp : tmp_list)
                info_list.push_back(tmp);
            }
    }

    for(info tmp : info_list){
        printf("%s\t\t", tmp.command);            
        printf("%s\t\t", tmp.pid);
        printf("%s\t\t", tmp.user);
        printf("%s\t\t", tmp.fd);
        printf("\n");
    }
    return 0;
}

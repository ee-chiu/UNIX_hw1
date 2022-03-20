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
    bzero(status_path, 30);
    strcpy(status_path, dir);
    strcat(status_path, "/status");
    FILE* status_file = fopen(status_path, "r");
    char* command = new char[1000];
    bzero(command, 1000);
    fscanf(status_file, "Name: %[^\n]", command);
    return command;
}

char* get_user(const char* dir){
    char* this_uid = get_uid(dir);
    FILE* passwd_file = Fopen("/etc/passwd", "r");
    char* user_info_line = new char[500];
    bzero(user_info_line, 500);
    char* user = new char[100];
    bzero(user, 100);
    char* uid = new char[10];
    bzero(uid, 10);
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

std::vector<char*> get_name_list(const char* file_name, const char* pid){
    char* path = new char[300];
    bzero(path, 300);
    strcpy(path, "/proc/");
    strcat(path, pid); 
    strcat(path, "/");
    strcat(path, file_name);
    if(!strcmp(file_name, "cwd") || !strcmp(file_name, "root") || !strcmp(file_name, "exe")) return name_link_case(path);
    if(!strcmp(file_name, "maps")) return name_maps_case(path);
    if(!strcmp(file_name, "fd")) return name_fd_case(path);
    return {};
}

std::vector<char*> get_fd_name(const char* file_name, const char* pid){
    if(!strcmp(file_name, "cwd")) return { (char*) "cwd" };
    if(!strcmp(file_name, "root")) return { (char*) "rtd" };
    if(!strcmp(file_name, "exe")) return { (char*) "txt" };
    if(!strcmp(file_name, "maps")) return { (char*) "mem" };
    if(!strcmp(file_name, "fd")) {
        char* path = new char [300];
        bzero(path, 300);
        strcpy(path, "/proc/");
        strcat(path, pid);
        strcat(path, "/");
        strcat(path, "fd");
        return fd_open_mode(path);
    }
    return { (char*) "error" };
}

char* get_type(const char* name, const char* fd){
    char* type = new char [10];
    bzero(type, 10);
    strcpy(type, "(empty)");
    if(!strcmp(fd, "NOFD")) {
        strcpy(type, "");
        return type;
    }
    if(strstr(name, "(Permission denied)") != NULL) {
        strcpy(type, "unknown");
        return type;
    }
    if(strstr(name, "pipe") != NULL){
        strcpy(type, "FIFO");
        return type;
    }
    if(strstr(name, "socket") != NULL){
        strcpy(type, "SOCK");
        return type;
    }
    struct stat buff;
    stat(name, &buff);
    if(S_ISDIR(buff.st_mode)) strcpy(type, "DIR");
    else if(S_ISREG(buff.st_mode)) strcpy(type, "REG");
    else if(S_ISCHR(buff.st_mode)) strcpy(type, "CHR");
    else if(S_ISFIFO(buff.st_mode)) strcpy(type, "FIFO");
    else if(S_ISSOCK(buff.st_mode)) strcpy(type, "SOCK");
    return type;
}


std::vector<info> get_info(const char *dir, const char *pid){
    std::vector<const char*> match_file_list = match_file(dir);
    char* command = get_command(dir);
    char* user = get_user(dir);
    std::vector<info> tmp_list;
    for(const char* file_name : match_file_list){
        std::vector<char*> fd_name_list = get_fd_name(file_name, pid);
        std::vector<char*> name_list = get_name_list(file_name, pid);
        for(size_t i = 0 ; i < fd_name_list.size() ; i++){
            char* fd = fd_name_list[i];
            if(!strcmp(fd, "mem")){
                for(char* name : name_list){
                    info tmp;
                    tmp.command = command;
                    tmp.pid = (char*) pid;
                    tmp.user = user;
                    tmp.fd = fd;
                    tmp.type = get_type(name, fd);
                    tmp.node = NULL;
                    tmp.name = name;
                    tmp_list.push_back(tmp); 
                }
                continue;
            }
            char* name = name_list[i];
            info tmp;
            tmp.command = command;
            tmp.pid = (char*) pid;
            tmp.user = user;
            tmp.fd = fd;
            tmp.type = get_type(name, fd);
            tmp.node = NULL;
            tmp.name = name;
            tmp_list.push_back(tmp);
        }
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
            std::vector<info> tmp_list = get_info(dir, (const char*) dirp->d_name);
            for(info tmp : tmp_list)
                info_list.push_back(tmp);
            }
    }

    for(info tmp : info_list){
        if(strcmp(tmp.command, "hw1")){
            printf("%s\t\t", tmp.command);            
            printf("%s\t\t", tmp.pid);
            printf("%s\t\t", tmp.user);
            printf("%s\t\t", tmp.fd);
            printf("%s\t\t", tmp.type);
            printf("%s\t\t", tmp.node);
            printf("%s\t\t", tmp.name);
            printf("\n");
        }
    }
    return 0;
}

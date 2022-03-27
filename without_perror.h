#include"std_map.h"
#include<string.h>
#include<vector>
#include<unistd.h>
#include<errno.h>
#include<map>
#include<fcntl.h>
const char* field_name = "COMMAND         PID             USER            FD              TYPE            NODE            NAME";

bool isnum(char* d_name){
    for(int i = 0 ; d_name[i] != '\0' ; i++){
        if(d_name[i] < '0' || d_name[i] > '9') return false;
    }

    return true;
}

int id2num(char* id){
    int num = 0;
    for(size_t i = 0 ; i < strlen(id) ; i++) num = num * 10 + (id[i] - '0');
    return num;
}
uid_t get_uid(const char *dir){
    char *status_path = new char[30];
    bzero(status_path, 30);
    strcpy(status_path, dir);
    strcat(status_path, "/status");
    FILE *status_file = fopen(status_path, "r");
    if(status_file == NULL) perror("fopen");
    char *uid_line = new char[1000];
    bzero(uid_line, 1000);
    for(int i = 1 ; i <= 9 ; i++)
        fgets(uid_line, 1000, status_file);
    uid_t uid;
    sscanf(uid_line, "Uid: %d", &uid);    
    return uid;
}

std::vector<const char*> match_file(const char* dir){
    std::map<const char*, bool, my_cmp> file_exists;
    std::vector<const char*> target_list = {"cwd", "root", "exe", "maps", "fd"};
    std::vector<const char*> match_file_list;
    DIR* dp = opendir(dir);
    if(dp == NULL) return {};
    struct dirent* dirp;
    while((dirp = readdir(dp)) != NULL) file_exists[dirp->d_name] = true;
    for(const char* t : target_list) if(file_exists[t]) match_file_list.push_back(t);
    return match_file_list;
}

std::vector<char*> fd_open_mode(const char* path){
    std::vector<char*> fd_list;
    DIR* dp = opendir(path);
    if(dp == NULL && errno == EACCES) return {(char*) "NOFD"};
    else if(dp == NULL) perror("fd opendir"); 
    struct dirent* dirp;
    while((dirp = readdir(dp)) != NULL){
        if(!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, "..")) continue;
        char* fd_path = new char [300];
        bzero(fd_path, 300);
        strcpy(fd_path, path);
        strcat(fd_path, "/");
        strcat(fd_path, dirp->d_name);
        FILE* fd_ = Fopen((const char*) fd_path, "r");
        char* pos = new char [30];
        fgets(pos, 30, fd_);
        char* flags = new char [30];
        bzero(flags, 30);
        fgets(flags, 30, fd_);
        mode_t st_mode = 0;
        sscanf(flags, "flags: %u", &st_mode);
        if(st_mode & O_RDWR) {
            char* fd = new char [10];
            bzero(fd, 10);
            strcpy(fd, dirp->d_name);
            strcat(fd, "u");
            fd_list.push_back(fd);
        }
        else if(st_mode & O_WRONLY) {
            char* fd = new char [10];
            bzero(fd, 10);
            strcpy(fd, dirp->d_name);
            strcat(fd, "w");
            fd_list.push_back(fd);
        }
        else {
            char* fd = new char [10];
            bzero(fd, 10);
            strcpy(fd, dirp->d_name);
            strcat(fd, "r");
            fd_list.push_back(fd);
        }
    }
    return fd_list; 
}

std::vector<char*> name_link_case(char* path){
    std::vector<char*> name_list;
    char* target_path = new char[300];
    bzero(target_path, 300);
    int r = readlink(path, target_path, 300);
    if(r < 0 && errno == EACCES) {
        strcat(path, " (Permission denied)");
        name_list.push_back(path);
        return name_list;
    }
    if(r < 0) return {}; //perror("readlink");
    name_list.push_back(target_path);
    return name_list;
}

std::vector<char*> name_maps_case(char* path){
    FILE* maps = fopen(path, "r"); 
    if(maps == NULL && errno == EACCES) return {};
    std::map<int, bool> num_exists;
    char* line = new char [500]; 
    bzero(line, 500);
    std::vector<char*> name_list;
    while(fgets(line, 500, maps) != NULL){
        char* id = new char [300];
        bzero(id, 300);
        char* save = NULL;
        strcpy(id, (const char*) strtok_r(line, " ", &save));
        for(int i = 1 ; i <= 4 ; i++) strcpy(id, (const char*) strtok_r(NULL, " ", &save));
        int num = id2num(id);
        if(num == 0) continue;
        if(num_exists.find(num) != num_exists.end()) continue;
        char* name = new char [300];
        bzero(name, 300);
        strcpy(name, strtok_r(NULL, " ", &save));
        if(name[strlen(name)-1] == '\n') name[strlen(name)-1] = '\0';
        name_list.push_back(name);
        num_exists[num] = true;
    }
    return name_list;
}

std::vector<char*> name_fd_case(const char* path){
    std::vector<char*> name_fd_list;
    DIR* dp = opendir(path);
    if(dp == NULL && errno == EACCES) { 
        char* fail_path = new char [300];
        bzero(fail_path, 300);
        strcpy(fail_path, path);
        strcat(fail_path, " (Permission denied)");
        return { fail_path };
    }
    else if(dp == NULL) return {}; //perror("fd opendir");
    struct dirent* dirp;
    while((dirp = readdir(dp)) != NULL){
        if(!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, "..")) continue;
        char* fd_path = new char [300];
        bzero(fd_path, 300);
        strcpy(fd_path, path);
        strcat(fd_path, "/");
        strcat(fd_path, dirp->d_name);
        char* target_path = new char [300];
        bzero(target_path, 300);
        int r = readlink(fd_path, target_path, 300);
        if(r < 0) return {}; //perror("readlink");
        name_fd_list.push_back(target_path);
    }
    return name_fd_list;
}
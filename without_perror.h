#include"std_map.h"
#include<string.h>
#include<vector>
#include<unistd.h>
#include<errno.h>
#include<map>
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
char* get_uid(const char *dir){
    char *status_path = new char[30];
    strcpy(status_path, dir);
    strcat(status_path, "/status");
    FILE *status_file = fopen(status_path, "r");
    if(status_file == NULL) perror("fopen");
    char *uid_line = new char[1000];
    for(int i = 1 ; i <= 9 ; i++)
        fgets(uid_line, 1000, status_file);
    char *uid = new char[10];
    sscanf(uid_line, "Uid: %s", uid);    
    return uid;
}

std::vector<const char*> match_file(const char* dir){
    std::map<const char*, bool, my_cmp> file_exists;
    std::vector<const char*> target_list = {"cwd", "root", "exe", "maps", "fd"};
    std::vector<const char*> match_file_list;
    DIR* dp = Opendir(dir);
    struct dirent* dirp;
    while((dirp = readdir(dp)) != NULL) file_exists[dirp->d_name] = true;
    for(const char* t : target_list) if(file_exists[t]) match_file_list.push_back(t);
    return match_file_list;
}

char* fd_name(const char* file_name){
    if(!strcmp(file_name, "cwd")) return (char*) "cwd";
    if(!strcmp(file_name, "root")) return (char*) "rtd";
    if(!strcmp(file_name, "exe")) return (char*) "txt";
    if(!strcmp(file_name, "maps")) return (char*) "mem";
    return (char*) "error";
}

std::vector<char*> name_link_case(char* path){
    std::vector<char*> name_list;
    char* target_path = new char[300];
    int r = readlink(path, target_path, 300);
    if(r < 0 && errno == EACCES) {
        strcat(path, " (Permission denied)");
        name_list.push_back(path);
        return name_list;
    }
    if(r < 0) perror("readlink");
    name_list.push_back(target_path);
    return name_list;
}

std::vector<char*> name_maps_case(char* path){
    FILE* maps = fopen(path, "r"); 
    if(maps == NULL && errno == EACCES) return {};
    std::map<int, char*> num2name;
    char* line = new char [500]; 
    while(fgets(line, 500, maps) != NULL){
        char* id = new char [300];
        char* save = NULL;
        strcpy(id, (const char*) strtok_r(line, " ", &save));
        for(int i = 1 ; i <= 4 ; i++) strcpy(id, (const char*) strtok_r(NULL, " ", &save));
        int num = id2num(id);
        if(num == 0) continue;
        char* name = new char [300];
        strcpy(name, strtok_r(NULL, " ", &save));
        if(name[strlen(name)-1] == '\n') name[strlen(name)-1] = '\0';
        num2name[num] = name;
    }
    std::vector<char*> name_list;
    for(std::map<int, char*>::iterator it = num2name.begin() ; it != num2name.end() ; it++){
        name_list.push_back(it->second); 
    }
    return name_list;
}

std::vector<char*> fd_open_mode(const char* path){
    std::vector<char*> fd_list;
    DIR* dp = opendir(path);
    if(dp == NULL && errno == EACCES) return {};
    else if(dp == NULL) perror("fd opendir"); 
    struct dirent* dirp;
    while((dirp = readdir(dp)) != NULL){
        if(!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, "..")) continue;
        char* fd_path = new char [300];
        strcpy(fd_path, path);
        strcat(fd_path, "/");
        strcat(fd_path, dirp->d_name);
        struct stat buff;
        lstat(fd_path, &buff);
        if( (buff.st_mode & S_IRUSR) && (buff.st_mode & S_IWUSR)) {
            char* fd = new char [10];
            strcpy(fd, dirp->d_name);
            strcat(fd, "u");
            fd_list.push_back(fd);
        }
        else if(buff.st_mode & S_IRUSR){
            char* fd = new char [10];
            strcpy(fd, dirp->d_name);
            strcat(fd, "r");
            fd_list.push_back(fd);
        }
        else if(buff.st_mode & S_IWUSR){
            char* fd = new char [10];
            strcpy(fd, dirp->d_name);
            strcat(fd, "w");
            fd_list.push_back(fd);
        }
    }
    return fd_list; 
}
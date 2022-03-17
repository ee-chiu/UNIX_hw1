#include<string.h>
#include<vector>

bool isnum(char* d_name){
    for(int i = 0 ; d_name[i] != '\0' ; i++){
        if(d_name[i] < '0' || d_name[i] > '9') return false;
    }

    return true;
}

char* get_uid(const char *dir){
    char *status_path = new char[30];
    strcpy(status_path, dir);
    strcat(status_path, "/status");
    FILE *status_file = Fopen(status_path, "r");
    char *uid_line = new char[1000];
    for(int i = 1 ; i <= 9 ; i++)
        fgets(uid_line, 100, status_file);
    char *uid = new char[10];
    sscanf(uid_line, "Uid: %s", uid);    
    return uid;
}

std::vector<const char*> match_file(const char* dir){
    std::vector<const char*> target_list = {"cwd", "root", "exe", "maps"};
    std::vector<const char*> match_file_list;
    DIR* dp = Opendir(dir);
    struct dirent* dirp;

    while((dirp = readdir(dp)) != NULL){
        for(const char* t : target_list){
            if(!strcmp(t, dirp->d_name)) match_file_list.push_back(t);
        }
    }

    return match_file_list;
}

char* fd_name(const char* file_name){
    if(!strcmp(file_name, "cwd")) return (char*) "cwd";
    if(!strcmp(file_name, "root")) return (char*) "rtd";
    if(!strcmp(file_name, "exe")) return (char*) "txt";
    if(!strcmp(file_name, "maps")) return (char*) "mem";
    return (char*) "error";
}